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

# Set LLVM version and environment
ARG LLVM_VERSION=18.1.8
ARG LLVM_PROJECT=llvmorg-${LLVM_VERSION}
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
    ninja-build python3-distutils curl zlib1g-dev libffi-dev

# Clone LLVM project repo at the specified version (shallow clone)
WORKDIR /src
RUN git clone --depth 1 --branch ${LLVM_PROJECT} https://github.com/llvm/llvm-project.git

# Configure LLVM build with Clang and LLD for all required targets
WORKDIR /src/llvm-project/build
RUN cmake -G Ninja ../llvm \
    -DLLVM_ENABLE_PROJECTS="clang;lld" \
    -DCMAKE_BUILD_TYPE=Release \
    -DLLVM_TARGETS_TO_BUILD="X86;AArch64" \
    -DCMAKE_INSTALL_PREFIX=/opt/llvm

# Build and install LLVM
RUN ninja && ninja install

# Add LLVM binaries to system PATH
ENV PATH="/opt/llvm/bin:$PATH"

# Install LLVM from repositories (deprecated)
# RUN echo "deb https://apt.llvm.org/bookworm llvm-toolchain-bookworm-18 main" \
#         > /etc/apt/sources.list.d/llvm.list && \
#     wget -qO /etc/apt/trusted.gpg.d/llvm.asc \
#         https://apt.llvm.org/llvm-snapshot.gpg.key && \
#     apt update -qqy && \
#     apt install -qqy -t llvm-toolchain-bookworm-18 llvm-18-dev clang-18 clang-tidy-18 clang-format-18 lld-18 libc++-18-dev libc++abi-18-dev && \
#     for f in /usr/lib/llvm-*/bin/*; do ln -sf "$f" /usr/bin; done && \
#     ln -sf clang /usr/bin/cc && \
#     ln -sf clang /usr/bin/c89 && \
#     ln -sf clang /usr/bin/c99 && \
#     ln -sf clang++ /usr/bin/c++ && \
#     ln -sf clang++ /usr/bin/g++ && \
#     rm -rf /var/lib/apt/lists/*

# Add custom marker for environment identification
RUN touch /.dittobytes-transpilers-env

# Create shared work directory (used as a volume)
RUN mkdir -p /tmp/workdir

# Set working directory to shared volume
WORKDIR /tmp/workdir