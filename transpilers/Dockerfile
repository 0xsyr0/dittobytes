# GNU General Public License, version 2.0.
#
# Copyright (c) 2025 Tijme Gommers (@tijme).
#
# This source code file is part of Dittobytes. Dittobytes is 
# licensed under GNU General Public License, version 2.0, and 
# you are free to use, modify, and distribute this file under 
# its terms. However, any modified versions of this file must 
# include this same license and copyright notice.

FROM debian:bookworm-slim AS intermediate

# Update & upgrade
RUN apt update -qqy
RUN apt upgrade -qqy

# Install APT dependencies
RUN apt install -qqy --no-install-recommends \
        gnupg2 wget ca-certificates apt-transport-https \
        autoconf automake cmake dpkg-dev file make patch libc6-dev mingw-w64 nano python3 python3-pip xxd \
        build-essential subversion python3-dev libncurses5-dev libxml2-dev libedit-dev swig doxygen graphviz xz-utils gdb git 

# Install LLVM
RUN echo "deb https://apt.llvm.org/bookworm llvm-toolchain-bookworm-18 main" \
        > /etc/apt/sources.list.d/llvm.list && \
    wget -qO /etc/apt/trusted.gpg.d/llvm.asc \
        https://apt.llvm.org/llvm-snapshot.gpg.key && \
    apt update -qqy && \
    apt install -qqy -t llvm-toolchain-bookworm-18 clang-18 clang-tidy-18 clang-format-18 lld-18 libc++-18-dev libc++abi-18-dev && \
    for f in /usr/lib/llvm-*/bin/*; do ln -sf "$f" /usr/bin; done && \
    ln -sf clang /usr/bin/cc && \
    ln -sf clang /usr/bin/c89 && \
    ln -sf clang /usr/bin/c99 && \
    ln -sf clang++ /usr/bin/c++ && \
    ln -sf clang++ /usr/bin/g++ && \
    rm -rf /var/lib/apt/lists/*

# Define identification for the containers
RUN touch /.dittobytes-transpilers-env

# Create output directory (expected to be a shared volume)
RUN mkdir -p /tmp/workdir

WORKDIR /tmp/workdir