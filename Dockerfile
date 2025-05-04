# GNU General Public License, version 2.0.
#
# Copyright (c) 2025 Tijme Gommers (@tijme).
#
# This source code file is part of Dittobytes. Dittobytes is 
# licensed under GNU General Public License, version 2.0, and 
# you are free to use, modify, and distribute this file under 
# its terms. However, any modified versions of this file must 
# include this same license and copyright notice.

# Use minimal Debian base image
FROM debian:bookworm-slim AS intermediate

# Set environment
ENV DEBIAN_FRONTEND=noninteractive

# Update package lists
RUN apt update -qqy

# Install APT dependencies
RUN apt install -qqy --no-install-recommends \
    gnupg2 wget ca-certificates apt-transport-https \
    autoconf automake cmake dpkg-dev file make patch \
    libc6-dev mingw-w64 nano python3 python3-pip xxd \
    build-essential subversion python3-dev \
    libncurses5-dev libxml2-dev libedit-dev \
    swig doxygen graphviz xz-utils gdb git \
    ninja-build python3-distutils curl zlib1g-dev libffi-dev \
    gcc-aarch64-linux-gnu binutils-aarch64-linux-gnu libc6-dev-arm64-cross \
    qemu-user qemu-user-static 

# Install Python dependencies
RUN python3 -m pip install -r scripts/requirements.txt --break-system-packages

# Install MacOS SDK
WORKDIR /opt
RUN git clone --depth 1 https://github.com/tijme/forked-dittobytes-macos-sdk.git macos-sdk

# Install LLVM (For Windows & Linux X86/ARM64)
WORKDIR /opt
RUN wget https://github.com/mstorsjo/llvm-mingw/releases/download/20240619/llvm-mingw-20240619-msvcrt-ubuntu-20.04-x86_64.tar.xz
RUN tar -xf llvm-mingw-20240619-msvcrt-ubuntu-20.04-x86_64.tar.xz
RUN mv llvm-mingw-20240619-msvcrt-ubuntu-20.04-x86_64 llvm-winlin
RUN rm llvm-mingw-20240619-msvcrt-ubuntu-20.04-x86_64.tar.xz

# Clone LLVM project repo at the specified version (shallow clone)
WORKDIR /opt
RUN git clone --depth 1 --branch release/18.x https://github.com/tijme/forked-dittobytes-llvm-project.git llvm-source

# Configure LLVM build with Clang and LLD for all required targets
WORKDIR /opt/llvm-source/build
RUN cmake -G Ninja ../llvm \
    -DLLVM_ENABLE_PROJECTS="clang;lld" \
    -DCMAKE_BUILD_TYPE=Release \
    -DLLVM_TARGETS_TO_BUILD="X86;AArch64" \
    -DCMAKE_INSTALL_PREFIX=/opt/llvm \
    -DBUILD_SHARED_LIBS=On

# Build and install LLVM
# If the following installation commands fail on Windows with a gRPC connection error,
# run these Dockerfile commands in a WSL container: `wsl --install --distribution Debian`.
RUN ninja
RUN ninja install

# Copy X86 & AArch64 header files manually
# TODO: Find out why this is required.
RUN cp -r /src/llvm-project/build/lib/Target/X86/ /opt/llvm/include/llvm/Target/
RUN cp -r /src/llvm-project/llvm/lib/Target/X86/ /opt/llvm/include/llvm/Target/
RUN cp -r /src/llvm-project/build/lib/Target/AArch64/ /opt/llvm/include/llvm/Target/
RUN cp -r /src/llvm-project/llvm/lib/Target/AArch64/ /opt/llvm/include/llvm/Target/

# Add LLVM binaries to system PATH
ENV PATH="/opt/llvm/bin:$PATH"

# Add custom marker for environment identification
RUN touch /tmp/.dittobytes-env-all-encompassing

# Create shared work directory (used as a volume)
RUN mkdir -p /tmp/workdir

# Set working directory to shared volume
WORKDIR /tmp/workdir