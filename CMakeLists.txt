#
#   mdrOS Cmake file
#

cmake_minimum_required(VERSION 3.10)
project(mdrOS)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

#
#   x86_64 Bootloader build targets
#
set(x86_64_bootloader_stage1 src/arch/x86_64/boot/stage1)
set(x86_64_bootloader_stage2 src/arch/x86_64/boot/stage2)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(NOT DEFINED TARGET_ARCH)
    message(STATUS "Architecture not specified, building for host")
    execute_process(COMMAND uname -m OUTPUT_VARIABLE TARGET_ARCH OUTPUT_STRIP_TRAILING_WHITESPACE)
    message(STATUS "Autodetected ${TARGET_ARCH}")
endif()

#
#   macOS specific toolchain
#
include(include/Cmake/macos.cmake)

#
#   Two stage bootloader for x86_64
#
add_subdirectory(${x86_64_bootloader_stage1})
add_subdirectory(${x86_64_bootloader_stage2})

