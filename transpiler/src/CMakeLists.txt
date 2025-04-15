# GNU General Public License, version 2.0.
#
# Copyright (c) 2025 Tijme Gommers (@tijme).
#
# This source code file is part of Dittobytes. Dittobytes is 
# licensed under GNU General Public License, version 2.0, and 
# you are free to use, modify, and distribute this file under 
# its terms. However, any modified versions of this file must 
# include this same license and copyright notice.

cmake_minimum_required(VERSION 3.13.4)
project(MyMachineFunctionPassPlugin)

set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(LLVM 20.1.2 REQUIRED CONFIG)

# Include directories specified by LLVM in the project's include path
include_directories(${LLVM_INCLUDE_DIRS})
# Include definitions specified by LLVM in the project's options
add_definitions(${LLVM_DEFINITIONS})
link_directories(${LLVM_LIBRARY_DIR})

# Use the same C++ standard as LLVM does
set(CMAKE_CXX_STANDARD 17 CACHE STRING "")

# LLVM is normally built without RTTI. Be consistent with that.
if(NOT LLVM_ENABLE_RTTI)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

add_library(PolymorphicTranspiler SHARED PolymorphicTranspiler.cpp)

target_link_libraries(PolymorphicTranspiler ${llvm_libs})

