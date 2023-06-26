set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION "0.0.0")
cmake_minimum_required(VERSION 3.7)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Prevent CMake from testing the toolchain
set(CMAKE_C_COMPILER_FORCED 1)
set(CMAKE_CXX_COMPILER_FORCED 1)

set(ARM_TARGET_TRIPLET arm-none-eabi)

find_program(CMAKE_C_COMPILER NAMES "${ARM_TARGET_TRIPLET}-gcc")
find_program(CMAKE_CXX_COMPILER NAMES "${ARM_TARGET_TRIPLET}-g++")
find_program(CMAKE_ASM_COMPILER NAMES "${ARM_TARGET_TRIPLET}-gcc")
find_program(CMAKE_AR NAMES "${ARM_TARGET_TRIPLET}-ar")
find_program(CMAKE_OBJCOPY NAMES "${ARM_TARGET_TRIPLET}-objcopy")
find_program(CMAKE_OBJDUMP NAMES "${ARM_TARGET_TRIPLET}-objdump")
find_program(SIZE NAMES "${ARM_TARGET_TRIPLET}-size")
find_program(XFEL NAMES xfel PATHS "${CMAKE_SOURCE_DIR}/tools/xfel")

PROJECT("T113" C CXX ASM)

file(GLOB_RECURSE CORE_SOURCES
    "app/*.c"
    "target/*.c"
    "app/*.cpp"
    "target/*.cpp"
)

file(GLOB ASM_SOURCES
    "target/t113-s3-bootloader/crt_CortexA_CPUn.S"
    "target/t113-s3-bootloader/crt_CortexA.S"
)

include_directories(
    "${CMAKE_SOURCE_DIR}/lib"
    "${CMAKE_SOURCE_DIR}/target/t113-s3-bootloader"
)

set(SOURCES
    ${SOURCES}
    ${ASM_SOURCES}
    ${CORE_SOURCES}
)

set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/target/t113-s3-bootloader/linker_script.ld")

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

target_compile_options(${PROJECT_NAME}.elf PRIVATE
    -mcpu=cortex-a7
    -mfpu=neon-vfpv4
    -mfloat-abi=hard
    -Og
    -g
    -Wall
    -ffunction-sections
    -fdata-sections
    -fstack-usage
    -fno-math-errno
    -MMD
    -MP
    -ffreestanding

    -Werror
    -Wextra
    $<$<COMPILE_LANGUAGE:C>:-std=gnu2x>
    $<$<COMPILE_LANGUAGE:C>:-Wenum-conversion>
    $<$<COMPILE_LANGUAGE:CXX>:-std=gnu++20>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-use-cxa-atexit>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
    $<$<COMPILE_LANGUAGE:CXX>:-Wno-volatile>

    -DCPUSTYLE_T113
)

target_link_options(${PROJECT_NAME}.elf PRIVATE
    -mcpu=cortex-a7
    -mfpu=neon-vfpv4
    -mfloat-abi=hard
    -T ${LINKER_SCRIPT}
    -nostdlib
    -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map
    -Wl,--gc-sections -static
    -fno-math-errno
    -fstack-usage
    -Wl,--start-group -lstdc++ -lsupc++
    -Wl,--end-group
    -Wl,--print-memory-usage -T ${LINKER_SCRIPT}
)

# Print size
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${SIZE} ${PROJECT_NAME}.elf)

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

# Make binary file.
add_custom_command(
    OUTPUT ${HEX_FILE}
    DEPENDS ${PROJECT_NAME}.elf
    COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
    COMMENT "Building ${HEX_FILE}"
)

add_custom_command(
    OUTPUT ${BIN_FILE}
    DEPENDS ${PROJECT_NAME}.elf
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
    COMMENT "Building ${BIN_FILE}"
)

add_custom_target(
    reset
    ${XFEL} reset
    COMMENT "Reset the target via xfel"
)

add_custom_target(
    flash_xfel_to_ddr
    ${XFEL} ddr t113-s3 && ${XFEL} write 0x40000000 ${BIN_FILE} && ${XFEL} exec 0x40000000
    DEPENDS ${BIN_FILE}
    COMMENT "Flash the target DDR mem via xfel"
)
