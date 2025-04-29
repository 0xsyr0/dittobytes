<p align="center">
    <img src="https://gist.githubusercontent.com/tijme/c77f321c8dacd6d8ce8e0f9e2ab8c719/raw/5443341235c2d82b244081499fbb17177d60604d/dittobytes.svg" alt="Dittobytes Logo" />
</p>
<p align="center">
    <a href="https://github.com/tijme/dittobytes/blob/master/LICENSE.md"><img src="https://img.shields.io/badge/License-MPL%20V2.0-ec0545?style=for-the-badge&labelColor=c7003b" alt="License badge" /></a>
    &nbsp;
    <img src="https://img.shields.io/github/actions/workflow/status/tijme/dittobytes/validation.yml?style=for-the-badge&labelColor=c7003b&color=ec0545" alt="Build status" />
</p>
<p align="center">
    <img src="https://gist.githubusercontent.com/tijme/ac043c7360ebcb89ac3be393a152dde0/raw/f7b8fbe8a16ab6897122829544721f7ad5d006b4/arch_amd64.svg" alt="AMD64 logo" height="45" />
    &nbsp;&nbsp;
    <img src="https://gist.githubusercontent.com/tijme/7262f114a2e018e323fd97837525f87d/raw/f10fbeb6f8d6cbbc1fa3bf864982f0dc120f651f/os_macos.svg" alt="MacOS logo" height="45" />
    &nbsp;&nbsp;
    <img src="https://gist.githubusercontent.com/tijme/7262f114a2e018e323fd97837525f87d/raw/f10fbeb6f8d6cbbc1fa3bf864982f0dc120f651f/os_windows.svg" alt="Windows logo" height="45" />
    &nbsp;&nbsp;
    <img src="https://gist.githubusercontent.com/tijme/7262f114a2e018e323fd97837525f87d/raw/86ecf9d13c520d181ce6e944dc76b3ddb623d697/os_linux.svg" alt="Linux logo" height="45" />
    &nbsp;&nbsp;
    <img src="https://gist.githubusercontent.com/tijme/a5e815ace37e12dc8e36060cc31cee4d/raw/397c225c23f4e20a26b88508d21262423465fb04/arch_arm64.svg" alt="ARCH64 logo" height="45" />
</p>
<p align="center">
    <b>Metamorphic cross-compilation of C-code to Truly Position Independent Code (PIC).</b>
    <br/>
    <sup>Built with ♥ by <a href="https://x.com/tijme">Tijme Gommers</a> – Buy me a coffee via <a href="https://www.paypal.me/tijmegommers">PayPal</a>.</sup>
    <br/>
</p>
<p align="center">
    <a href="#abstract">Abstract</a>
    •
    <a href="#requirements">Requirements</a>
    •
    <a href="#getting-started">Getting started</a>
    •
    <a href="#roadmap">Roadmap</a>
    •     
    <a href="#issues--requests">Issues & requests</a>
    •
    <a href="#license--copyright">License & copyright</a>
</p>
<hr>

# Abstract

Dittobytes compiles your C-code to truly Position Independent Code (PIC) for Windows, MacOS, and Linux, and both AMD64 and ARM64. It features a [metamorphic engine](https://en.wikipedia.org/wiki/Metamorphic_code) that ensures each compilation produces unique, functional shellcode. It does *not* rely on the classic decrypt stubs often seen in e.g. metamorphic compilations, and additionally it does *not* require reflective loaders such as Donut or sRDI as it compiles your C-code directly to PIC. A subsequent advantage is that the output size of the shellcode is extremely small (almost no overhead), and remains very simple.

<details>
    <summary>Technical details (click here)</summary>
    <hr>
    <p>
        Dittobytes uses a custom LLVM build with two custom <a href="https://llvm.org/docs/WritingAnLLVMNewPMPass.html">pass plugins</a>. Any compilation of your C-code using Dittobytes is done with this LLVM build. The first pass plugin (on intermediate level) ensures that certain constants (in e.g. data segments) are inlined, to aid in the development of Position Independent Code (PIC). The second pass plugin (on machine level) ensures that metamorphic transformations (e.g. instruction substitutions) are done, introducing randomness in the assembly code during compilation. This includes, but is not limited to:
        <ul>
            <li>Immediate substitution (e.g., <code>mov [reg], imm</code> → <code>mov [reg], encoded; xor [reg], key</code>).</li>
            <li>Instruction substitution (e.g., <code>mov [mem], imm</code> → <code>push imm; pop [mem]</code>).</li>
        </ul>
    </p>
    <hr>
</details>

Dittobytes ships with a minimal C-code file (`./beacon/main.c`) that can cross-compile to all supported platforms and architectures. Additionally, it ships with loaders (for each platform and architecture) that can be used to load and test your compiled shellcode.

![Dittobytes Overview](https://gist.githubusercontent.com/tijme/fe693d86e15cd015764bc6ea85225a66/raw/07037ee07c69d5425a2839150ccac15c8b96ce1f/overview.svg)

# Requirements

<details>
    <summary>Requirements to compile with Docker (easy)</summary>
    <hr>
    <p>
        Compiling the shellcode <a href="#compiling">can be easily done</a> via Docker, using the provided <code>Dockerfile</code>. However, this <code>Dockerfile</code> builds <a href="https://github.com/llvm/llvm-project">LLVM</a> from source, which requires quite some memory and disk space in your container. I got it to work with the following Docker resource settings (which seem to be a minimum for now):
        <br>
        <ul>
            <li>CPU limit: 8</li>
            <li>Memory limit: 10 GB</li>
            <li>Swap: 2 GB</li>
            <li>Disk usage limit: 1 TB (but this can likely be much lower)</li>
        </ul>
        If the container has insufficient resources, the build might fail with the following error: <code>ERROR: failed to solve: ResourceExhausted: process "/bin/sh -c ninja && ninja install" did not complete successfully: cannot allocate memory`</code>.
    </p>
    <hr>
</details>

<details>
    <summary>Requirements to compile on your host (advanced)</summary>
    <hr>
    <p>
        Compiling the shellcode <a href="#compiling">can be done</a> on your host as well. However, as you would need to build <a href="https://github.com/llvm/llvm-project">LLVM</a> from source, quite some memory and disk space is required. I got it to work with the following resources:
        <br>
        <ul>
            <li>CPU cores: 8</li>
            <li>Memory: 16 GB</li>
            <li>Disk space: 1 TB (but this can likely be much lower)</li>
        </ul>
    </p>
    <hr>
</details>

# Getting started

### 1. Prerequisites

<details>
    <summary>Cloning the repository</summary>
    <hr>
    <ul>
        <li>Use Git to clone this repo:<br><code>git clone https://github.com/tijme/dittobytes.git</code></li>
        <li><a href="https://github.com/tijme/dittobytes/blob/master/.github/laughing.gif">Review</a> the code so you know what you're compiling and running.</li>
        <li>Finally, cd into the project and start developing:<br><code>cd ./dittobytes/</code></li>
    </ul>
    <hr>
</details>

### 2. Overview

<details>
    <summary>Directory structure</summary>
    <hr>

    dittobytes/
    ├── beacon/                         # Your C-code that will compile to shellcode.
    │   ├── main.c                       
    ├── loaders/                        # Simple shellcode loaders for testing purposes.
    │   └── [platform]/
    │       ├── src/
    │       │   └── main.c
    │       └── lib/
    │           └── ...
    ├── builds/                         # Build dir containing loaders and your shellcodes.
    │   ├── beacon-[platform]-[arch].bin
    │   ├── loader-[platform]-[arch].[ext]
    │   └── ...
    ├── scripts/                        # Helper scripts used by the makefile(s).
    │   ├── extract-text-segment.py
    │   └── ...
    ├── tests/                          # C-code files used for feature testing.
    │   ├── [feature-test].c
    │   └── ...
    └── transpilers/                    # The LLVM plugins that act as metamorphic engine.
        ├── intermediate/
        │   └── src/
        │       ├── IntermediateTranspiler.cpp
        │       └── ...
        └── machine/
            └── src/
                ├── MachineTranspiler.cpp
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
    </p>
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
    <p>
        Clang and LLVM are used to cross-compile the loader and beacon. If you want to perform this compilation on your host machine, configure your host the same way as the Docker container is configured. Take a look at the <a href="https://github.com/tijme/dittobytes/blob/master/Dockerfile">Dockerfile</a> for reference. For now, there is no further documentation on setting up the environment on your host machine.
    </p>
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

<details>
    <summary>Running feature tests</summary>
    <hr>
    <p>
        Dittobytes comes pre-shipped with feature tests. A feature test is similar to a unit test, but tests from a large feature perspective, instead of a specific code unit perspective. Currently, you can only run feature tests for shellcodes that are compiled for the platform you are running the tests on. For example, in the Docker container only the Linux shellcode would be tested & verified.
        <br>
        <ul>
            <li>
                <li>Run the Docker container:<br><code>docker run --rm -v ".:/tmp/workdir" -it dittobytes</code></li>
                <li>Build the tests:<br><code>make test-suite-build</code></li>
                <li>Run the tests:<br><code>make test-suite-test</code></li>
            </li>
        </ul>
    </p>
    <hr>
</details>

### 6. Advanced

<details>
    <summary>Modification & compilation of the pre-shipped loaders</summary>
    <hr>
    <p>
        You can modify the pre-shipped loaders by editing the code in <code>./loaders/[platform]/src/main.c</code>, after which you can compile them using the following commands in the root of the Dittobytes project:
        <br>
        <ul>
            <li>Build the Docker container:<br><code>docker build -t dittobytes-loaders -f ./loaders/Dockerfile .</code></li>
            <li>Run the Docker container:<br><code>docker run --rm -v ".:/tmp/workdir" -it dittobytes-loaders</code></li>
            <li>Move to the right directory:<br><code>cd ./loaders/</code></li>
            <li>Compile the loaders:<br><code>make</code></li>
        </ul>
    </p>
    <hr>
</details>

<details>
    <summary>Modification & compilation of the pre-shipped transpilers</summary>
    <hr>
    <p>
        You can modify the pre-shipped transpiler(s) by editing the code in <code>./transpilers/[type]/src/[type].cpp</code>, after which you can compile them using the following commands in the root of the Dittobytes project:
        <br>
        <ul>
            <li>Build the Docker container:<br><code>docker build -t dittobytes-transpilers -f ./transpilers/Dockerfile .</code></li>
            <li>Run the Docker container:<br><code>docker run --rm -v ".:/tmp/workdir" -it dittobytes-transpilers</code></li>
            <li>Move to the right directory:<br><code>cd ./transpilers/[type]/</code></li>
            <li>Compile the transpiler:<br><code>make</code></li>
        </ul>
        Dittobytes ships with two transpilers. The first one is the intermediate transpiler that uses a modern <a href="https://llvm.org/docs/WritingAnLLVMNewPMPass.html">LLVM Function Pass</a> to inline constant variables otherwise located in <code>.rodata</code> segments. The second one is the machine transpiler that uses a legacy <a href="https://llvm.org/docs/WritingAnLLVMPass.html#the-machinefunctionpass-class">LLVM MachineFunction Pass</a> to perform the metamorphism.
    </p>
    <hr>
</details>

# Roadmap

There is no specific planning, so this might be more of a to-do or ideas list. The following unordered items would at least be nice to implement in Dittobytes.

* ✅ Done: Immediate substitution (e.g., `mov [reg], imm` → `mov [reg], encoded; xor [reg], key`).
* ⏳ ToDo: More substitution options for the existing immediate substitution module
* ⏳ ToDo: Instruction substitution (e.g., `mov [mem], imm` → `push imm; pop [mem]`).
* ⏳ ToDo: Register reallocation (randomize the registers to be used).
* ⏳ ToDo: Fake code insertion.

# Issues & requests

Issues or new feature requests can be reported via the [issue tracker](https://github.com/tijme/dittobytes/issues). Please make sure your issue or feature has not yet been reported by anyone else before submitting a new one.

# License & copyright

Copyright (c) 2025 Tijme Gommers. Dittobytes is released under the Mozilla Public License Version 2.0. View [LICENSE.md](https://github.com/tijme/dittobytes/blob/master/LICENSE.md) for the full license.
