# GNU General Public License, version 2.0.
#
# Copyright (c) 2025 Tijme Gommers (@tijme).
#
# This source code file is part of Dittobytes. Dittobytes is 
# licensed under GNU General Public License, version 2.0, and 
# you are free to use, modify, and distribute this file under 
# its terms. However, any modified versions of this file must 
# include this same license and copyright notice.

# Project & CMAKE settings
cmake_minimum_required(VERSION 3.25.0)
project(MachineTranspiler)

# Set specific compiler(s) & linker(s)
set(CMAKE_C_COMPILER "/opt/llvm/bin/clang")
set(CMAKE_CXX_COMPILER "/opt/llvm/bin/clang++")
set(CMAKE_LINKER "/opt/llvm/bin/lld")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find LLVM
find_package(LLVM 18.1.8 REQUIRED CONFIG)

# Print LLVM information
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Add LLVM definitions and includes
link_directories(${LLVM_LIBRARY_DIR})
add_definitions(${LLVM_DEFINITIONS})
include_directories(
    ${LLVM_INCLUDE_DIRS}
    ${LLVM_INCLUDE_DIRS}/llvm/Target/X86/
    ${LLVM_INCLUDE_DIRS}/llvm/Target/AArch64/
)

# Add the machine transpiler source code
add_library(MachineTranspiler SHARED
    MachineTranspiler.cpp
)

# Link against LLVM libraries
target_link_libraries(MachineTranspiler
    PRIVATE
    LLVMCore
    LLVMCodeGen
    LLVMSupport
    LLVMTarget
    LLVMAnalysis
    LLVMScalarOpts
    LLVMX86CodeGen
    LLVMX86Desc
    LLVMX86Info
)

# Set compilation flags
target_compile_options(MachineTranspiler PRIVATE
    -fno-rtti
)
