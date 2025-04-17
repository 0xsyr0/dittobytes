<p align="center">
    <img src="https://gist.githubusercontent.com/tijme/c77f321c8dacd6d8ce8e0f9e2ab8c719/raw/a1386dc7a0a4186a6946ed955946c90474a6607b/dittobytes.svg?raw=true&cache=1" alt="Dittobytes Logo" />
</p>
<p align="center">
    <a href="https://github.com/tijme/dittobytes/blob/master/LICENSE.md"><img src="https://img.shields.io/badge/License-MPL%20V2.0-ec0545?style=for-the-badge&labelColor=c7003b" alt="License badge" /></a>
    &nbsp;
    <img src="https://img.shields.io/badge/Platforms-Windows,%20Linux%20%26%20MacOS-ec0545?style=for-the-badge&labelColor=c7003b" alt="Platforms badge" />
    &nbsp;
    <img src="https://img.shields.io/badge/Architectures-AMD64%20%26%20ARM64-ec0545?style=for-the-badge&labelColor=c7003b" alt="Architectures badge" />
</p>
<p align="center">
    <img src="https://gist.githubusercontent.com/tijme/7262f114a2e018e323fd97837525f87d/raw/f10fbeb6f8d6cbbc1fa3bf864982f0dc120f651f/os_macos.svg" alt="MacOS logo" height="45" />
    &nbsp;&nbsp;
    <img src="https://gist.githubusercontent.com/tijme/7262f114a2e018e323fd97837525f87d/raw/f10fbeb6f8d6cbbc1fa3bf864982f0dc120f651f/os_windows.svg" alt="Windows logo" height="45" />
    &nbsp;&nbsp;
    <img src="https://gist.githubusercontent.com/tijme/7262f114a2e018e323fd97837525f87d/raw/f10fbeb6f8d6cbbc1fa3bf864982f0dc120f651f/os_linux.svg" alt="Linux logo" height="45" />
</p>
<p align="center">
    <b>Polymorphic cross-compilation of C-code to Truly Position Independent Code (PIC).</b>
    <br/>
    <sup>RIP In-Memory Signatures. Built with ♥ by <a href="https://x.com/tijme">Tijme Gommers</a> – Buy me a coffee via <a href="https://www.paypal.me/tijmegommers">PayPal</a>.</sup>
    <br/>
</p>
<p align="center">
    <a href="#abstract">Abstract</a>
    •
    <a href="#getting-started">Getting started</a>
    •
    <a href="#issues--requests">Issues & requests</a>
    •
    <a href="#license--copyright">License & copyright</a>
</p>
<hr>

# Abstract

Dittobytes compiles your C-code to truly Position Independent Code (PIC) for Windows, MacOS, and Linux, and both AMD64 and ARM64. It features a polymorphic engine that ensures each compilation produces unique, functional shellcode. It does *not* rely on the classic decrypt stubs often seen in polymorphic compilations, and additionally it does *not* require reflective loaders such as Donut or sRDI as it compiles your C-code directly to PIC. A subsequent advantage is that the output size of the shellcode is extremely small (almost no overhead), and remains very simple.

Dittobytes ships with a minimal C-code file (`./beacon/main.c`) that can cross-compile to all supported platforms and architectures. Additionally, it ships with loaders (for each platform and architecture) that can be used to load and test your compiled shellcode.

![Dittobytes Overview](https://gist.githubusercontent.com/tijme/fe693d86e15cd015764bc6ea85225a66/raw/07037ee07c69d5425a2839150ccac15c8b96ce1f/overview.svg)

# Getting started

### 1. Prerequisites

<details>
    <summary>Cloning the repository</summary>
    <hr>
    <ul>
        <li>Use Git to clone this repo:<br><code>git clone https://github.com/tijme/dittobytes.git</code></li>
        <li><a href="https://github.com/tijme/dittobytes/blob/master/.github/laughing.gif">Review</a> the code so you know what you're compiling and running.</li>
        <li>Finally, cd into the project and start developing:<br><code>cd dittobytes</code></li>
    </ul>
    <hr>
</details>

### 2. Overview

<details>
    <summary>Directory structure</summary>
    <hr>

    dittobytes/
    ├── beacon/                         # You C-code that will compile to shellcode.
    │   ├── main.c                       
    ├── loader/                         # Simple shellcode loader for testing purposes.
    │   └── [platform]/
    │       ├── src/
    │       │   └── main.c
    │       └── lib/
    │           └── ...
    ├── builds/                         # Build dir containing loaders and your shellcodes.
    │   ├── beacon-[platform]-[arch].bin
    │   ├── loader-[platform]-[arch].[ext]
    │   └── ...
    ├── scripts/                        # Helper scripts used by the makefile.
    │   ├── extract-text-segment.py
    │   └── ...
    └── transpiler/                     # The LLVM plugin that acts as polymorphic engine.
        ├── src/
        │   ├── PolymorphicTranspiler.cpp
        │   └── ...
        ├── build/
        │   └── libPolymorphicTranspiler.so
        └── ...

<hr>
</details>

### 3. Development

<details>
    <summary>The basics</summary>
    <hr>
    <p>
        You can modify <code>./beacon/main.c</code> however you like. Just keep the following in mind:
        <br>
        <ul>
            <li>The first function in your code must be named <code>EntryFunction</code>.</li>
            <li><code>EntryFunction</code> must literally (in order) be the first function in your code.</li>
            <li>You cannot use global variables (PIC limitation).</li>
            <li>You cannot use any data from other segments (PIC limitation).</li>
            <li>You must resolve any API function you want to use by yourself (PIC limitation).</li>
        </ul>
    </p>
    <p>
        The following example may give you some guidance. It fakes global variables by using a context struct that you would need to pass to any function you call. It initializes a string by using a char[] array. It calls another function by defining its definition first (as the other function needs to be defined before you can call it, but it cannot be the first function in your code).
    </p>
    <p>
        <a href="https://gist.github.com/tijme/01331c822a7e6e05fd192d4d3d118647">Example 'The Basics'</a>
    </p>
    <hr>
</details>

<details>
    <summary>A hello world</summary>
    <hr>
    <p>
        A hello world requires printing to the console, thus requiring an OS API call to e.g. <code>puts</code>. This is OS specific. For example, for Windows it would require loading <code>KERNEL32.dll</code>, ultimately resolving <code>LoadLibraryA</code> and <code>GetProcAddress</code>. With these two functions resolved, you can then load any function address, such as the address of <code>puts</code>.
    </p>
    <p>
        An example would become quite large, thus for now I'd like to forward you to <a href="https://github.com/tijme/relocatable">Relocatable</a>. This is a Position Independent Code (PIC) which pops a message box and calculator as example.
    <hr>
</details>

### 4. Compiling

<details>
    <summary>Compile your code using Docker (easy)</summary>
    <hr>
    <ul>
        <li>Build the Docker container:<br><code>docker build -t dittobytes .</code></li>
        <li>Run the Docker container:<br><code>docker run --rm -v ".:/tmp/workdir" -it dittobytes</code></li>
        <li>Compile your code:<br><code>make</code></li>
    </ul>
    <hr>
</details>

<details>
    <summary>Compile your code on your host (advanced)</summary>
    <hr>
    Clang and LLVM are used to cross-compile the loader and beacon. If you want to perform this compilation on your host machine, configure your host the same way as the Docker container is configured. Take a look at the <a href="https://github.com/tijme/dittobytes/blob/master/Dockerfile">Dockerfile</a> for reference. For now, there is no further documentation on setting up the environment on your host machine.
    <hr>
</details>

### 5. Testing

<details>
    <summary>Running your shellcode</summary>
    <hr>
    <ul>
        <li>
            Run and test your shellcode using the pre-shipped shellcode loader:
            <br>
            <code>./builds/loader-[os]-[arch].[ext] ./builds/beacon-[os]-[arch].bin</code>
        </li>
    </ul>
    <hr>
</details>

# Issues & requests

Issues or new feature requests can be reported via the [issue tracker](https://github.com/tijme/dittobytes/issues). Please make sure your issue or feature has not yet been reported by anyone else before submitting a new one.

# License & copyright

Copyright (c) 2025 Tijme Gommers. Dittobytes is released under the Mozilla Public License Version 2.0. View [LICENSE.md](https://github.com/tijme/dittobytes/blob/master/LICENSE.md) for the full license.
