#
#   mdrOS macOS specific Cmake configuration
#
#   We prefer Homebrew Clang but it does not really matter, the real problem
#   is using ld.lld and getting rid of platform specific macOS flags that are useless in this context.
#

execute_process(COMMAND uname OUTPUT_VARIABLE HOST_OS OUTPUT_STRIP_TRAILING_WHITESPACE)
if(HOST_OS STREQUAL "Darwin")
    message(STATUS "Building on Darwin/macOS, using  clang/lld")
    find_program(LINKER_LLD ld.lld)
    find_program(CLANG clang)

    if(NOT DEFINED LINKER_LLD)
        message(FATAL_ERROR "LLD is required to build on macOS !")
    else()
        message(STATUS "Found LLD in ${LINKER_LLD}")
    endif()

    if(NOT DEFINED CLANG)
        message(FATAL_ERROR "Clang is required to build on macOS !")
    else()
        message(STATUS "Found Clang in ${CLANG}")
    endif()

    set(CMAKE_C_LINK_EXECUTABLE 
        "${LINKER_LLD}  <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

    set(CMAKE_C_COMPILE_OBJECT
        "${CLANG} <DEFINES> <INCLUDES> <FLAGS> -o <OBJECT> -c <SOURCE>")
    
    set(CREATE_x64_ISO "zsh  -c \"cd ${CMAKE_SOURCE_DIR} && chmod +x scripts/createx64iso_mac.sh && scripts/createx64iso_mac.sh\"")

endif()