#++
# CpuInfo
# Copyright (c) 2017-2018, Aidan Khoury. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# @file CMakeLists.txt
# @author Aidan Khoury (ajkhoury)
# @date 11/16/2018
#--
cmake_minimum_required(VERSION 3.4)

#
# Set the cmake module path
#
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

#
# Include cmake module
#
include(nasm)

#
# Use the NASM compiler
#
nasm_set_config(win64
        INCLUDES ${CMAKE_SOURCE_DIR}/include/nasm.inc)

#
# Define CpuInfo project
#
project(CpuInfo C)

#
# C Language Standard
#
set(CMAKE_C_STANDARD 99) # C99 Standard
#set(CMAKE_C_STANDARD_LIBRARIES "") # Don't link with any libraries

#
# Project include directories
#
include_directories(
        ${CMAKE_SOURCE_DIR}/include
        )

#
# If using MSVC, disable the annoying C4159 warning.
#
if(MSVC)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4159")
endif()

#
# Project source files.
#
set(CpuInfo_ASM_SOURCE_FILES
        #src/x64/cpuid.asm # Use if intrinsics are not available
        #src/i386/cpuid.asm # Use if intrinsics are not available
        )
set(CpuInfo_SOURCE_FILES
        src/pif.c
        src/main.c
        )

add_executable(CpuInfo ${CpuInfo_ASM_SOURCE_FILES} ${CpuInfo_SOURCE_FILES})

set_source_files_properties(${CpuInfo_ASM_SOURCE_FILES} PROPERTIES LANGUAGE ASM_NASM)
set_source_files_properties(${CpuInfo_SOURCE_FILES} PROPERTIES LANGUAGE C)