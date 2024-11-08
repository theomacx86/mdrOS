#
#   mdrOS first stage bootloader CMakeLists.txt
#

cmake_minimum_required(VERSION 3.14)
project(stage1_bootloader)

enable_language(ASM_NASM)

set(CMAKE_ASM_NASM_FLAGS "-f bin")

set(SOURCE_FILE ${CMAKE_CURRENT_SOURCE_DIR}/stage1.asm)

# Specify the output binary file name
set(OUTPUT_BINARY stage1_bootloader)

# Add a custom command to compile the assembly file to a flat binary
# Cmake doesn't really respect the output file format even if we ask it to, so let's force it
# It also prevents the linker from linking a flat binary ...
add_custom_command(
    OUTPUT ${OUTPUT_BINARY}
    COMMAND ${CMAKE_ASM_NASM_COMPILER} ${CMAKE_ASM_NASM_FLAGS} -o ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${OUTPUT_BINARY} ${SOURCE_FILE}
    DEPENDS ${SOURCE_FILE}
    COMMENT "Assembling first stage bootloader ${SOURCE_FILE} ${OUTPUT_BINARY}"
)

add_custom_target(compile_flat_binary ALL DEPENDS ${OUTPUT_BINARY})