<p align="center">
    <img src="https://gist.githubusercontent.com/tijme/c77f321c8dacd6d8ce8e0f9e2ab8c719/raw/a1386dc7a0a4186a6946ed955946c90474a6607b/dittobytes.svg" alt="Dittobytes Logo" />
</p>
<p align="center">
    <a href="https://github.com/tijme/dittobytes/releases"><img src="https://img.shields.io/github/v/release/tijme/dittobytes?style=for-the-badge&labelColor=c7003b&color=ec0545" alt="Latest Dittobytes release" /></a>
    &nbsp;
    <a href="https://github.com/tijme/dittobytes/actions"><img src="https://img.shields.io/github/actions/workflow/status/tijme/dittobytes/validation.yml?style=for-the-badge&labelColor=c7003b&color=ec0545" alt="Latest Dittobytes status" /></a>
    &nbsp;
    <a href="https://github.com/tijme/dittobytes/blob/master/LICENSE.md"><img src="https://img.shields.io/badge/License-MPL%20V2.0-ec0545?style=for-the-badge&labelColor=c7003b" alt="Dittobytes license badge" /></a>
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
    <sup>Built with ♥ by <a href="https://www.linkedin.com/in/tijme/">Tijme Gommers</a> – Buy me a coffee via <a href="https://www.paypal.me/tijmegommers">PayPal</a>.</sup>
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

Dittobytes compiles your C-code to truly Position Independent Code (PIC) for Windows, MacOS, and Linux, and both AMD64 and ARM64. It features a [metamorphic engine](https://en.wikipedia.org/wiki/Metamorphic_code) that ensures each compilation produces unique, functional shellcode. It does *not* rely on the classic decrypt stubs often seen in e.g. polymorphic compilations, and additionally it does *not* require reflective loaders such as Donut or sRDI as it compiles your C-code directly to PIC. A subsequent advantage is that the output size of the shellcode is extremely small (almost no overhead), and remains very simple.

<p align="center">
    <img src="https://gist.githubusercontent.com/tijme/8a1e77e82316df8b41d62e8cdaca2ddb/raw/8fc6ef90df9bbff70c6fedf73c63ff6b07d449a1/dittobytes-example-diff.png" alt="Metamorphication example with Dittobytes" />
    <br>
    <sup>Figure A: Metamorphication example by Dittobytes</sup>
</p>

<p>
    Dittobytes uses a custom LLVM build with two transpilers. Any compilation of your C-code using Dittobytes is done with this LLVM build. The first transpiler uses a modern <a href="https://llvm.org/docs/WritingAnLLVMNewPMPass.html">LLVM Function Pass</a> (on intermediate level) to inline constant variables otherwise located in e.g. <code>.rodata</code> segments (this aids the development of Position Independent Code). The second one is the machine transpiler that uses a legacy <a href="https://llvm.org/docs/WritingAnLLVMPass.html#the-machinefunctionpass-class">LLVM MachineFunction Pass</a> to perform the metamorphic transformations (e.g. instruction substitutions), introducing randomness in the assembly code during compilation. Check the <a href="#roadmap">roadmap</a> for all implemented (and yet to implement) transformations.
</p>

The pre-shippped minimal C-code file (`./beacon/main.c`) can cross-compile to all supported platforms and architectures. Additionally, it ships with loaders (for each platform and architecture) that can be used for testing purposes.

# Requirements

<details>
    <summary>Requirements to compile with Docker (<strong>easy</strong>)</summary>
    <hr>
    <p>
        You can <a href="#4-compiling">easily compile</a> <code>./beacon/main.c</code> via Docker, using the provided <code>Dockerfile</code>. However, this <code>Dockerfile</code> builds a custom version of <a href="https://github.com/tijme/forked-dittobytes-llvm-project/tree/release/18.x">LLVM</a> from source, which requires quite some memory and disk space to be allocated by Docker. The build takes around 2.5 hours. I got it to work with the following Docker resource configuration.
        <blockquote>⚠️ If Docker cannot allocate enough resources, the build might fail with an error like <code>ResourceExhausted: cannot allocate memory</code>.</blockquote>
        <ul>
            <li>Set CPU limit to: <code>8</code>.</li>
            <li>Set memory limit to: <code>10 GB</code>.</li>
            <li>Set swap to: <code>2 GB</code>.</li>
            <li>Set disk usage limit: <code>1 TB</code> (though this can likely be much lower).</li>
        </ul>
    </p>
    <hr>
</details>

<details>
    <summary>Requirements to compile on your host (<strong>advanced</strong>)</summary>
    <hr>
    <p>
        You can <a href="#4-compiling">compile</a> <code>./beacon/main.c</code> on your host as well. However, as you would need to build a custom version of <a href="https://github.com/tijme/forked-dittobytes-llvm-project/tree/release/18.x">LLVM</a> from source, quite some memory and disk space is required. The build takes around 2.5 hours. I got it to work with the following resources.
        <ul>
            <li>CPU cores: <code>8</code>.</li>
            <li>Memory: <code>10 GB</code>.</li>
            <li>Disk space: <code>1 TB</code> (though this can likely be much lower).</li>
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
        <li>Clone this repository using Git:<br><pre><code>git clone https://github.com/tijme/dittobytes.git</code></pre></li>
        <li>Manually <a href="https://github.com/tijme/dittobytes/blob/master/.github/laughing.gif">review</a> the code so you know what you're compiling and running.</li>
        <li>Finally, move into the project directory and start developing:<br><pre><code>cd ./dittobytes/</code></pre></li>
    </ul>
    <hr>
</details>

<details>
    <summary>Building the build tools in a Docker container (<strong>easy</strong>)</summary>
    <hr>
    <p>
        The easiest way to use Dittobytes is via Docker. For this, you need to build a Docker image using the provided <code>Dockerfile</code>.
        <br>
        <ul>
            <li>Build the Docker image:<br><pre><code>docker buildx build -t dittobytes .</code></pre></li>
            <li>Building the image will take around 2.5 hours as LLVM needs to be built from source.</li>
        </ul>
    </p>
    <hr>
</details>

<details>
    <summary>Installing the build tools on your host instead (<strong>advanced</strong>)</summary>
    <hr>
    <p>
        Clang and LLVM are used to cross-compile the beacon, loaders and transpilers. If you want to perform this compilation on your host machine, configure your host the same way as the Docker container is configured. Take a look at the <a href="https://github.com/tijme/dittobytes/blob/master/Dockerfile">Dockerfile</a> or <a href="https://github.com/tijme/dittobytes/blob/master/.github/workflows/validation.yml">GitHub Workflow</a> for reference. For now, there is no further documentation on setting up the environment on your host machine.
    </p>
    <hr>
</details>

### 2. Overview

<details>
    <summary>Directory structure</summary>
    <hr>

    dittobytes/
    ├── beacon/                         # Your C-code that will compile to shellcode.
    │   ├── main.c                       
    ├── loaders/                        # Simple shellcode loaders for testing purposes (pre-built).
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
        The following example may give you some guidance. It simulates global variables by using a context struct that you would need to pass to any function you call. It initializes a string by using a char[] array. It calls another function by defining its definition first (as the other function needs to be defined before you can call it, but it cannot be the first function in your code).
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
    <summary>Compile your code</summary>
    <hr>
    <ul>
        <li>If using Docker, run a Dittobytes container:<br><code>docker run --rm -v ".:/tmp/workdir" -it dittobytes</code></li>
        <li>Compile your code:<br><code>make</code></li>
    </ul>
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
        <li>If using Docker, run a Dittobytes container:<br><code>docker run --rm -v ".:/tmp/workdir" -it dittobytes</code></li>
            <li>Build the tests:<br><code>make test-suite-build</code></li>
            <li>Run the tests:<br><code>make test-suite-test</code></li>
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
            <li>If using Docker, run a Dittobytes container:<br><code>docker run --rm -v ".:/tmp/workdir" -it dittobytes</code></li>
            <li>Compile the loaders:<br><code>make loaders</code></li>
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
            <li>If using Docker, run a Dittobytes container:<br><code>docker run --rm -v ".:/tmp/workdir" -it dittobytes</code></li>
            <li>Compile the transpilers:<br><code>make transpilers</code></li>
        </ul>
        Dittobytes ships with two transpilers. The first one is the intermediate transpiler that uses a modern <a href="https://llvm.org/docs/WritingAnLLVMNewPMPass.html">LLVM Function Pass</a> to inline constant variables otherwise located in <code>.rodata</code> segments. The second one is the machine transpiler that uses a legacy <a href="https://llvm.org/docs/WritingAnLLVMPass.html#the-machinefunctionpass-class">LLVM MachineFunction Pass</a> to perform the metamorphism.
    </p>
    <hr>
</details>

# Roadmap

There is no specific planning, so this might be more of a to-do or ideas list. The following items (unordered) would at least be nice to implement in Dittobytes.

<details>
    <summary>Metamorphication</summary>
    <hr>
    <table>
        <tr>
            <th>Status</th>
            <th>Description</th>
            <th>Source</th>
        </tr>
        <tr>
            <td>✅ Done</td>
            <td>Immediate substitution (e.g., <code>mov [reg], imm</code> → <code>mov [reg], encoded; xor [reg], key</code>)</td>
            <td><a href="https://github.com/tijme/dittobytes/blob/master/transpilers/machine/src/modules/modify_mov_immediate/ModifyMovImmediateModule.cpp">MachineTranspiler</a></td>
        </tr>
        <tr>
            <td>⏳ ToDo</td>
            <td>More substitution options for the existing immediate substitution module.</td>
            <td>&nbsp;</td>
        </tr>
        <tr>
            <td>⏳ ToDo</td>
            <td>ToDo: Instruction substitution (e.g., <code>`mov [mem], imm`</code> → <code>`push imm; pop [mem]`</code>).</td>
            <td>&nbsp;</td>
        </tr>
        <tr>
            <td>✅ Done</td>
            <td>Register reallocation (randomize the registers to be used).</td>
            <td><a href="https://github.com/tijme/forked-dittobytes-llvm-project/blob/main/llvm/lib/CodeGen/RegAllocGreedy.cpp">LLVM source</a></td>
        </tr>
        <tr>
            <td>⏳ ToDo</td>
            <td>Insertion of fake basic blocks based on assembly from trusted software.</td>
            <td>&nbsp;</td>
        </tr>
    </table>
    <hr>
</details>

<details>
    <summary>Other To-Dos</summary>
    <hr>
    <ul>
        <li>Test & report Levenshtein distance of different shellcode compilations.</li>
        <li>Implement a more complex and larger feature test to verify correctness of compiled shellcode.</li>
    </ul>
    <hr>
</details>

# Issues & requests

Issues or new feature requests can be reported via the [issue tracker](https://github.com/tijme/dittobytes/issues). Please make sure your issue or feature has not yet been reported by anyone else before submitting a new one.

# License & copyright

Copyright &copy; 2025 Tijme Gommers. Dittobytes is released under the Mozilla Public License Version 2.0. View [LICENSE.md](https://github.com/tijme/dittobytes/blob/master/LICENSE.md) for the full license. Dittobytes depends on various open-source components which all have their own license and copyright.