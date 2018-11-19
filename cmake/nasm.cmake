#
# Copyright (c) 2017 Aidan Khoury. All rights reserved.
#
# Simple cmake script to resolve and configure the NASM compiler.
#
# @file nasm.cmake
# @author Aidan Khoury
# @date 9/17/2017
#

#
# Sets up NASM compiler configuration and command line flags.
#
macro(nasm_set_config format)
    cmake_parse_arguments(ARG "" "" "INCLUDE_DIRS;INCLUDES" ${ARGN})

    #
    # Enable the NASM compiler.
    #
    set(CMAKE_ASM_NASM_OBJECT_FORMAT ${format})
    enable_language(ASM_NASM)
    if(NOT CMAKE_ASM_NASM_COMPILER_LOADED)
        message(FATAL_ERROR "NASM assembler not found!")
    endif(NOT CMAKE_ASM_NASM_COMPILER_LOADED)

    #
    # Construct command line include directories.
    #
    set(CMAKE_NASM_INCLUDE_DIRS "")
    foreach(INCLUDE_DIR ${ARG_INCLUDE_DIRS})
        #message(STATUS "include dir = ${INCLUDE_DIR}")
        set(CMAKE_NASM_INCLUDE_DIRS "-i${INCLUDE_DIR}/ ${CMAKE_NASM_INCLUDE_DIRS}")
    endforeach()

    #
    # Construct command line includes.
    #
    set(CMAKE_NASM_INCLUDES "")
    foreach(INCLUDE_FILE ${ARG_INCLUDES})
        #message(STATUS "include = ${INCLUDE_FILE}")
        set(CMAKE_NASM_INCLUDES "-p${INCLUDE_FILE} ${CMAKE_NASM_INCLUDES}")
    endforeach()

    #
    # Setup global NASM flags
    #  - Add the underscore prefix when compiling with msvc.
    #
    if(MSVC)
        set(CMAKE_ASM_NASM_FLAGS "-O0 -g ${CMAKE_NASM_INCLUDES} --prefix _ -w-orphan-labels")
    else()
        set(CMAKE_ASM_NASM_FLAGS "-O0 -g ${CMAKE_NASM_INCLUDES} -w-orphan-labels")
    endif()

    #
    # Setup NASM compiler command line.
    #  - Add the -l listing file option
    #
    set(CMAKE_ASM_NASM_COMPILE_OBJECT
            "<CMAKE_ASM_NASM_COMPILER> <INCLUDES> <FLAGS> ${CMAKE_NASM_INCLUDE_DIRS}-f${format} -o <OBJECT> <SOURCE>")
    message(STATUS "CMAKE_ASM_NASM_COMPILE_OBJECT=${CMAKE_ASM_NASM_COMPILE_OBJECT}")

    #
    # Setup NASM linker command line if using MSVC.
    #  - Use vc link.exe linker
    #
    if(MSVC)
        set(CMAKE_ASM_NASM_LINK_EXECUTABLE
                "<CMAKE_LINKER> <OBJECTS> /out:<TARGET> <CMAKE_ASM_NASM_LINK_FLAGS> <LINK_FLAGS> <LINK_LIBRARIES>")
    endif()

endmacro()