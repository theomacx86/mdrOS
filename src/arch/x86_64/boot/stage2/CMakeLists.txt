#
#   mdrOS second stage bootloader CMakeLists.txt
#

cmake_minimum_required(VERSION 3.14)
project(stage2_bootloader)

enable_language(ASM_NASM)
enable_language(C)

set(BUILD_SHARED_LIBS OFF)

set(CMAKE_C_FLAGS "-target x86_64-unknown-none -DPLATFORM_MACOS -Wall -nostdlib -ffreestanding")
set(CMAKE_EXE_LINKER_FLAGS "--oformat=binary -t ${CMAKE_SOURCE_DIR}/targets/x86_64/bootloader_linker.ld")

file(GLOB BOOTLOADER_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
file(GLOB BOOTLOADER_ASM  "${CMAKE_CURRENT_SOURCE_DIR}/*.asm")

set(SOURCE_FILE ${BOOTLOADER_SRC})

set(ASM_FILES ${BOOTLOADER_ASM})

add_executable(stage2_bootloader ${SOURCE_FILE} ${ASM_FILES})

target_include_directories(stage2_bootloader PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(stage2_bootloader PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
