<p align="center">
    <img src="https://gist.githubusercontent.com/tijme/c77f321c8dacd6d8ce8e0f9e2ab8c719/raw/029692c26cb1dd1c05f1c4544a96d333544b9f3a/dittobytes.svg" alt="Dittobytes Logo" />
</p>
<p align="center">
    <a href="https://github.com/tijme/dittobytes/releases"><img src="https://img.shields.io/github/v/release/tijme/dittobytes?style=for-the-badge&labelColor=850447&color=ba0745" alt="Latest Dittobytes release" /></a>
    &nbsp;
    <a href="https://github.com/tijme/dittobytes/actions"><img src="https://img.shields.io/github/actions/workflow/status/tijme/dittobytes/validation.yml?style=for-the-badge&labelColor=850447&color=ba0745" alt="Latest Dittobytes status" /></a>
    &nbsp;
    <a href="https://github.com/tijme/dittobytes/blob/master/LICENSE.md"><img src="https://img.shields.io/badge/License-MPL%20V2.0-ba0745?style=for-the-badge&labelColor=850447" alt="Dittobytes license badge" /></a>
</p>
<p align="center">
    <img src="https://gist.githubusercontent.com/tijme/ac043c7360ebcb89ac3be393a152dde0/raw/2f4bfb6f639419b49c9a2e3bfa440fdfd7576949/arch_amd64.svg" alt="AMD64 logo" width="50" height="50" />
    &nbsp;&nbsp;
    <img src="https://gist.githubusercontent.com/tijme/7262f114a2e018e323fd97837525f87d/raw/5e58faa4765f054e86e7c774be06bacb6e630b7b/os_macos.svg" alt="MacOS logo" width="50" height="50" />
    &nbsp;&nbsp;
    <img src="https://gist.githubusercontent.com/tijme/7262f114a2e018e323fd97837525f87d/raw/5e58faa4765f054e86e7c774be06bacb6e630b7b/os_windows.svg" alt="Windows logo" width="50" height="50" />
    &nbsp;&nbsp;
    <img src="https://gist.githubusercontent.com/tijme/7262f114a2e018e323fd97837525f87d/raw/5e58faa4765f054e86e7c774be06bacb6e630b7b/os_linux.svg" alt="Linux logo" width="50" height="50" />
    &nbsp;&nbsp;
    <img src="https://gist.githubusercontent.com/tijme/a5e815ace37e12dc8e36060cc31cee4d/raw/2f6fba67d2d597294de5ccaec48d1325f0c76354/arch_arm64.svg" alt="ARCH64 logo" width="50" height="50" />
</p>
<p align="center">
    <b>Metamorphic cross-compilation of C++ & C-code to PIC, BOF & EXE.</b>
    <br/>
    <sup>Built with ♥ by <a href="https://www.linkedin.com/in/tijme/">Tijme Gommers</a> – Buy me a coffee via <a href="https://dittobytes.com/sponsor-via-paypal">PayPal</a> or <a href="https://dittobytes.com/sponsor-via-bunq">Bunq</a>.</sup>
    <br/>
</p>
<p align="center">
    <a href="#-hardware-requirements">Requirements</a>
    &nbsp;•&nbsp;
    <a href="#-getting-started">Getting started</a>
    &nbsp;•&nbsp;
    <a href="#-advanced-usage">Advanced usage</a>
    &nbsp;•&nbsp;
    <a href="#-roadmap">Metamorphications</a>
    &nbsp;•&nbsp;
    <a href="#-limitations">Limitations</a>
    &nbsp;•&nbsp;
    <a href="#-issues--requests">Issues</a>
    &nbsp;•&nbsp;
    <a href="#-license--copyright">Copyright</a>
</p>
<hr>

Dittobytes compiles your C-code to truly Position Independent Code (PIC) for Windows, MacOS, and Linux, and both AMD64 and ARM64. It features a [metamorphic engine](https://en.wikipedia.org/wiki/Metamorphic_code) that ensures each compilation produces unique, functional shellcode. It does *not* rely on the classic decrypt stubs often seen in e.g. polymorphic compilations, and additionally it does *not* require reflective loaders such as Donut or sRDI as it can compile your C-code directly to PIC. A subsequent advantage is that the output size of the shellcode is extremely small (almost no overhead), and remains very simple.

<table align=center>
    <tr>
        <td align=center>Original</td>
        <td align=center></td>
        <td align=center>Metamorphicated (example)</td>
    </tr>
    <tr>
        <td>

```diff
# push    rbp
# mov     rbp, rsp
- push    r15
- push    r11
- sub     rsp, 40h
- mov     rax, 2073692073696874h


- mov     [rbp+var_1B], rax
```

</td>
<td align=center>→</td>
<td>
        
```diff
# push    rbp
# mov     rbp, rsp
+ push    r9
+ push    r15
+ sub     rsp, 38h
+ mov     r14, 6E055571BF8F0D8Eh
+ mov     rdx, 4E763C51CCE665FAh
+ xor     rdx, r14
+ mov     [rbp+var_33], rdx
```

</td>
</tr>
</table>
<p align=center><sup>Illustration 1: Example metamorphications by Dittobytes (left and right are functionally equivalent).</sup></p>

<p>
    Dittobytes uses a custom LLVM build with two transpilers. Any compilation of your code using Dittobytes is done with this LLVM build. The first transpiler uses a modern <a href="https://llvm.org/docs/WritingAnLLVMNewPMPass.html">LLVM Function Pass</a> (on intermediate level) to inline constant variables otherwise located in e.g. <code>.rodata</code> segments (this aids the development of Position Independent Code). The second one is the machine transpiler that uses a legacy <a href="https://llvm.org/docs/WritingAnLLVMPass.html#the-machinefunctionpass-class">LLVM MachineFunction Pass</a> to perform the metamorphic transformations (e.g. instruction substitutions), introducing randomness in the assembly code during compilation. Check the <a href="#-roadmap">roadmap</a> for all implemented (and yet to implement) metamorphic transformations.
</p>

The pre-shippped minimal C-code file (`./code/beacon.c`) can cross-compile to all supported platforms (Windows, Linux & MacOS), architectures (AMD64 & ARM64) and formats (PIC, BOF, EXE). Additionally, Dittobytes ships with loaders (for each platform and architecture) that can be used for testing purposes.

<h1><img src="https://gist.githubusercontent.com/tijme/c77f321c8dacd6d8ce8e0f9e2ab8c719/raw/029692c26cb1dd1c05f1c4544a96d333544b9f3a/logo-emblem.svg" width=30 height=30 /> Hardware requirements</h1>

<details>
    <summary>Requirements to compile with Docker<br><sup>Difficulty: <strong>easy</strong></summary>
    <hr>
    <p>
        You can <a href="#compiling">easily compile</a> <code>./code/beacon.c</code> via Docker, using the provided <code>Dockerfile</code>. However, this <code>Dockerfile</code> builds a custom version of <a href="https://github.com/tijme/forked-dittobytes-llvm-project/tree/release/18.x">LLVM</a> from source, which requires quite some memory and disk space to be allocated by Docker. The build takes around 2.5 hours. I got it to work with the following Docker resource configuration.
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
    <summary>Requirements to compile with Windows Subsystem for Linux<br><sup>Difficulty: <strong>intermediate</strong></summary>
    <hr>
    <p>
        You can <a href="#compiling">compile</a> <code>./code/beacon.c</code> via Windows Subsystem for Linux (WSL). However, as you would need to build a custom version of <a href="https://github.com/tijme/forked-dittobytes-llvm-project/tree/release/18.x">LLVM</a> from source, quite some memory and disk space is required. The build takes around 2.5 hours. I got it to work with the following resources.
        <ul>
            <li>CPU cores: <code>8</code>.</li>
            <li>Memory: <code>10 GB</code>.</li>
            <li>Disk space: <code>1 TB</code> (though this can likely be much lower).</li>
        </ul>
    </p>
    <hr>
</details>

<details>
    <summary>Requirements to compile on your host<br><sup>Difficulty: <strong>advanced</strong></summary>
    <hr>
    <p>
        You can <a href="#compiling">compile</a> <code>./code/beacon.c</code> on your Linux host as well. However, as you would need to build a custom version of <a href="https://github.com/tijme/forked-dittobytes-llvm-project/tree/release/18.x">LLVM</a> from source, quite some memory and disk space is required. The build takes around 2.5 hours. I got it to work with the following resources.
        <ul>
            <li>CPU cores: <code>8</code>.</li>
            <li>Memory: <code>10 GB</code>.</li>
            <li>Disk space: <code>1 TB</code> (though this can likely be much lower).</li>
        </ul>
    </p>
    <hr>
</details>

<h1><img src="https://gist.githubusercontent.com/tijme/c77f321c8dacd6d8ce8e0f9e2ab8c719/raw/029692c26cb1dd1c05f1c4544a96d333544b9f3a/logo-emblem.svg" width=30 height=30 /> Getting started</h1>

### Overview

<details>
    <summary>Directory structure</summary>
    <hr>

    dittobytes/
    ├── code/                               # Your C-code that will compile to shellcode.
    │   ├── beacon.c                        # Example file that you can compile using Dittobytes.
    ├── build/                              # Build dir containing loaders and your shellcodes.
    │   ├── beacon-[platform]-[arch].raw    # Your C-code compiled to raw shellcode (.text segment only).
    │   ├── beacon-[platform]-[arch].obj    # Your C-code compiled to BOF/COFF format.
    │   ├── beacon-[platform]-[arch].exe    # Your C-code compiled to executable format.
    │   ├── loader-[platform]-[arch]        # Pre-built raw shellcode loaders for testing purposes.
    │   └── ...
    └── ditto/                              # Internal files supporting the Dittobytes project.
        ├── loaders/                        # Simple shellcode loaders for testing purposes (pre-built).
        │   └── [platform]/
        │       ├── src/
        │       │   └── main.c
        │       └── lib/
        │           └── ...
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

### Preparing

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
    <summary>Building the build tools in a Docker container<br><sup>Difficulty: <strong>easy</strong></summary>
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
    <summary>Installing the build tools on Windows Subsystem for Linux instead<br><sup>Difficulty: <strong>intermediate</strong></summary>
    <hr>
    <p>
        If you are on Windows, a more performant option to build the build tools is to use Windows Subsystem for Linux (WSL). However, in contrast to Docker, the installation of the build tools is a manual process.
        <br>
        <ul>
            <li>First of all, run a WSL Debian container:<br><pre><code>wsl -d Debian</code></pre></li>
        </ul>
        <p>
            Custom versions of Clang and LLVM are eventually used to cross-compile your code, the loaders and the transpilers. Performing this compilation in WSL requires you to configure your WSL the same way as the Docker container is configured. Take a look at the <a href="https://github.com/tijme/dittobytes/blob/master/Dockerfile">Dockerfile</a> or <a href="https://github.com/tijme/dittobytes/blob/master/.github/workflows/validation.yml">GitHub Workflow</a> for reference. Follow the exact same steps as in one of those files. For now, there is no further documentation on setting up the environment in WSL.
        </p>
    </p>
    <hr>
</details>

<details>
    <summary>Installing the build tools on your host instead<br><sup>Difficulty: <strong>advanced</strong></summary>
    <hr>
    <p>
        Custom versions of Clang and LLVM are used to cross-compile your code, the loaders and the transpilers. If you want to perform this compilation on your host machine, configure your host the same way as the Docker container is configured. Take a look at the <a href="https://github.com/tijme/dittobytes/blob/master/Dockerfile">Dockerfile</a> or <a href="https://github.com/tijme/dittobytes/blob/master/.github/workflows/validation.yml">GitHub Workflow</a> for reference. Follow the exact same steps as in one of those files. For now, there is no further documentation on setting up the environment on your host machine.
    </p>
    <hr>
</details>

### Developing

<details>
    <summary>The basics</summary>
    <hr>
    <p>
        You can modify <code>./code/beacon.c</code> however you like. Just keep the following in mind:
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
        The following example may give you some guidance. It simulates global variables by using a context struct that you would need to pass to any function you call. It initializes a string by using a <code>char[]</code> array. It calls another function by defining its definition first (as the other function needs to be defined before you can call it, but it cannot be the first function in your code).
    </p>
    <p>
        <a href="https://github.com/tijme/dittobytes/blob/master/code/examples/example-basics/example-basics.c">Example 'The Basics' (<code>example-basics.c</code>)</a>
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
        An example would become quite large, thus for now I'd like to forward you to example file below. It is a Position Independent Code (PIC) for Windows AMD64 & ARM64 which pops a calculator as example.
    </p>
    <p>
        <a href="https://github.com/tijme/dittobytes/blob/master/code/examples/example-calc/example-calc.c">Example 'Popping Calc' (<code>example-calc.c</code>)</a>
    </p>
    <hr>
</details>

### Compiling

<details>
    <summary>Compile your code</summary>
    <hr>
    <ul>
        <li>If using Docker, run a Dittobytes container:<br><code>docker run --rm -v ".:/tmp/workdir" -it dittobytes</code></li>
        <li>Compile your code:<br><code>make</code></li>
    </ul>
    <hr>
</details>

### Outputs

<details>
    <summary>Position Independent Code (<code>.raw</code>)</summary>
    <hr>
    <p>Dittobytes was originally designed to output Truly Position Independent Code (PIC). Simply put, PIC consists of the executable assembly instructions from the <code>.text</code> segment of an executable binary, without any reference to other segments or absolute memory addresses.</p>
    <p>Dittobytes generates <code>.raw</code> files for Windows, Linux and MacOS (and both AMD64 and ARM64).</p>
    <hr>
</details>

<details>
    <summary>Beacon Object File (<code>.obj</code>)</summary>
    <hr>
    <p>In the process of creating Position Independent Code, Dittobytes creates an <code>.obj</code> file (COFF/ELF format). This file is later used to extract the <code>.text</code> segment (<code>.raw</code>) from, or create the executable format (<code>.exe</code>) with. However, the <code>.obj</code> file itself can be used as Cobalt Strike (or any other C&C framework) Beacon Object File (BOF) as well.</p>
    <p>Dittobytes generates <code>.obj</code> files for Windows, Linux and MacOS (and both AMD64 and ARM64).</p>
    <hr>
</details>

<details>
    <summary>Executable/Clickable (<code>.exe</code>)</summary>
    <hr>
    <p>Dittobytes uses the generated Position Independent Code (PIC) in the <code>.obj</code> file to eventually generate an executable/clickable file format (<code>.exe</code>). This means that all executables generated by Dittobytes solely contain Position Independent Code (PIC). For example, constants are inlined instead of stored in the <code>.rodata</code> segment.</p>
    <p>Dittobytes generates <code>.exe</code> files for Windows, Linux and MacOS (and both AMD64 and ARM64).</p>
    <hr>
</details>

### Testing

<details>
    <summary>Running your shellcode</summary>
    <hr>
    <ul>
        <li>
            Run and test your shellcode using the pre-shipped shellcode loader:
            <br>
            <code>./build/loader-[os]-[arch].[ext] ./build/beacon-[os]-[arch].raw</code>
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

<h1><img src="https://gist.githubusercontent.com/tijme/c77f321c8dacd6d8ce8e0f9e2ab8c719/raw/029692c26cb1dd1c05f1c4544a96d333544b9f3a/logo-emblem.svg" width=30 height=30 /> Advanced usage</h1>

<details>
    <summary>Using C++ instead of C for your code</summary>
    <hr>
    <p>
        You can easily utilize functionality of C++ by renaming your code file from <code>./code/beacon.c</code> to <code>./code/beacon.cpp</code>. Just make sure to prepend the <code>EntryFunction</code> in the file with <code>extern "C"</code>. Also ensure that the <code>SOURCE_PATH</code> option in the <code>makefile</code> points to the new filename. Do note that you <b>cannot</b> use functionality from external libraries such as <code>libstdc++</code> or <code>libc++</code>. This means you <b>cannot</b> make use of e.g. <code>std::string</code> ⚠️.
    </p>
    <p>
        <a href="https://github.com/tijme/dittobytes/blob/master/code/examples/example-cpp/example-cpp.cpp">Example 'C++ instead of C-code' (<code>example-cpp.c</code>)</a>
    </p>
    <p>Compiling C++ code in Dittobytes works exactly the same as compiling regular C-code.</p>
    <ul>
        <li>If using Docker, run a Dittobytes container:<br><code>docker run --rm -v ".:/tmp/workdir" -it dittobytes</code></li>
        <li>Then compile your code:<br><code>make</code></li>
    </ul>
    <hr>
</details>

<details>
    <summary>Compiling a Cobalt Strike Beacon Object File (BOF)</summary>
    <hr>
    <p>
        To compile a Beacon Object File (BOF) for Cobalt Strike or any other Command & Control framework, copy <code>./code/examples/example-bof/example-bof.c</code> to <code>./code/beacon.c</code>. Then adjust the source code to your needs.
    </p>
    <p>
        <a href="https://github.com/tijme/dittobytes/blob/master/code/examples/example-bof/example-bof.c">Example 'Beacon Object File' (<code>example-bof.c</code>)</a>
    </p>
    <p>Remember to solely compile to the <code>BOF/COFF</code> format using the <code>make</code> command (see below example) ⚠️.</p>
    <ul>
        <li>If using Docker, run a Dittobytes container:<br><code>docker run --rm -v ".:/tmp/workdir" -it dittobytes</code></li>
        <li>Then compile your code:<br><code>make beacon-bof-win-amd64</code></li>
    </ul>
    <hr>
</details>

<details>
    <summary>Modification & compilation of the pre-shipped loaders</summary>
    <hr>
    <p>
        You can modify the pre-shipped loaders by editing the code in <code>./ditto/loaders/[platform]/src/main.c</code>, after which you can compile them using the following commands in the root of the Dittobytes project:
        <br>
        <ul>
            <li>If using Docker, run a Dittobytes container:<br><code>docker run --rm -v ".:/tmp/workdir" -it dittobytes</code></li>
            <li>Compile the loaders:<br><code>make ditto-loaders</code></li>
        </ul>
    </p>
    <hr>
</details>

<details>
    <summary>Modification & compilation of the pre-shipped transpilers</summary>
    <hr>
    <p>
        You can modify the pre-shipped transpiler(s) by editing the code in <code>./ditto/transpilers/[type]/src/[type].cpp</code>, after which you can compile them using the following commands in the root of the Dittobytes project:
        <br>
        <ul>
            <li>If using Docker, run a Dittobytes container:<br><code>docker run --rm -v ".:/tmp/workdir" -it dittobytes</code></li>
            <li>Compile the transpilers:<br><code>make ditto-transpilers</code></li>
        </ul>
        Dittobytes ships with two transpilers. The first one is the intermediate transpiler that uses a modern <a href="https://llvm.org/docs/WritingAnLLVMNewPMPass.html">LLVM Function Pass</a> to inline constant variables otherwise located in <code>.rodata</code> segments. The second one is the machine transpiler that uses a legacy <a href="https://llvm.org/docs/WritingAnLLVMPass.html#the-machinefunctionpass-class">LLVM MachineFunction Pass</a> to perform the metamorphism.
    </p>
    <hr>
</details>

<details>
    <summary>Compiling & running one specific feature test</summary>
    <hr>
    <p>
        The test-suite commands in the makefile usually compile and test all feature tests (cross-os and cross-architecture). If you want to test just one specific feature test, or if you want to to test build artifacts for a specific os or architecture, use the commands below. You can adjust the <code>TEST_*</code> arguments to your needs.
        <br>
        <ul>
            <li>If using Docker, run a Dittobytes container:<br><code>docker run --rm -v ".:/tmp/workdir" -it dittobytes</code></li>
            <li>Build the test(s):<br><code>make TEST_OS=win TEST_ARCH=arm64 TEST_SOURCE_PATH=./ditto/tests/all/all/3_metamorphication_010_transform_nullifications.c TEST_METAMORPHICATION=transform_nullifications test-suite-build</code></li>
            <li>Run the test(s):<br><code>make TEST_OS=win TEST_ARCH=arm64 TEST_SOURCE_PATH=./ditto/tests/all/all/3_metamorphication_010_transform_nullifications.c TEST_METAMORPHICATION=transform_nullifications test-suite-test</code></li>
        </ul>
        The above example would build the feature test <code>3_metamorphication_010_transform_nullifications.c</code> for Windows ARM64. This may result in many build artifacts (<code>[amount of feature tests] × [amount of os's] × [amount of arch's] × [amount of metamorphications]</code>), in this case 1 (<code>1 × 1 × 1 × 1</code>). The second command verifies the build artifacts based on the <code>@verify</code> statements in the feature test source code file(s).
    </p>
    <hr>
</details>

<h1><img src="https://gist.githubusercontent.com/tijme/c77f321c8dacd6d8ce8e0f9e2ab8c719/raw/029692c26cb1dd1c05f1c4544a96d333544b9f3a/logo-emblem.svg" width=30 height=30 /> Roadmap</h1>

There is no specific planning, so this might be more of a to-do or ideas list. The following items (unordered) would at least be nice to implement in Dittobytes.

<details>
    <summary>Metamorphications</summary>
    <hr>
    <ul>
        <li>
            ✅ <b>RandomizeRegisterAllocation</b>: Randomizes the allocation order of CPU registers.
            <br/>
            <sup>Implemented in <a href="https://github.com/tijme/dittobytes/releases/tag/release-1.0.0">release 1.0.0</a>.</sup>
            <table>
                <tr>
                    <td align=center>Original</td>
                    <td></td>
                    <td align=center>Metamorphicated (example)</td>
                </tr>
                <tr>
                    <td>

```diff
- mov     r15, 4BC202D525C93492h
- mov     r10, 6BB16BF556A05CE6h
- xor     r10, r15
- mov     [rbp+var_2B], r10
- mov     r9, 3081F61A6A1776DDh
- mov     r11, 44EF9F7B066756BCh
```

</td>
<td align=center>→</td>
<td>

```diff
+ mov     r13, 4BC202D525C93492h
+ mov     r14, 6BB16BF556A05CE6h
+ xor     r14, r13
+ mov     [rbp+var_2B], r14
+ mov     r10, 3081F61A6A1776DDh
+ mov     r9, 44EF9F7B066756BCh
```

</td>
                </tr>
            </table>
        </li>
        <li>
            ✅ <b>TransformRegMovImmediates</b>: Substitutes instructions that move an immediate value to a register in various ways.
            <br/>
            <sup>Implemented in <a href="https://github.com/tijme/dittobytes/releases/tag/release-1.0.0">release 1.0.0</a>.</sup>
            <table>
                <tr>
                    <td align=center>Original</td>
                    <td></td>
                    <td align=center>Metamorphicated (example)</td>
                </tr>
                <tr>
                    <td>

```diff
- mov     rcx, 2073692073696874
```

</td>
<td align=center>→</td>
<td>

```diff
+ mov     rax, 4BC202D525C93492h
+ mov     rcx, 6BB16BF556A05CE6h
+ xor     rcx, rax
```

</td>
                </tr>
            </table>
        </li>
        <li>
            ✅ <b>TransformStackMovImmediates</b>: Substitutes instructions that move an immediate value to the stack in various ways.
            <br/>
            <sup>Implemented in <a href="https://github.com/tijme/dittobytes/releases/tag/release-1.0.9">release 1.0.9</a>.</sup>
            <table>
                <tr>
                    <td align=center>Original</td>
                    <td></td>
                    <td align=center>Metamorphicated (example)</td>
                </tr>
                <tr>
                    <td>

```diff
- mov     [rbp+var_8], 0FFFFFFFFFFFFFFFFh
```

</td>
<td align=center>→</td>
<td>

```diff
+ mov     rax, 0D3F57F4h
+ mov     [rbp+var_8], 0FFFFFFFFF2C0A80Bh
+ xor     [rbp+var_8], rax
```

</td>
                </tr>
            </table>
        </li>
        <li>
            ✅ <b>TransformNullifications</b>: Substitutes various instructions that nullify a register.
            <br/>
            <sup>Implemented in <a href="https://github.com/tijme/dittobytes/releases/tag/release-1.0.2">release 1.0.2</a>.</sup>
            <table>
                <tr>
                    <td align=center>Original</td>
                    <td></td>
                    <td align=center>Metamorphicated (example)</td>
                </tr>
                <tr>
                    <td>

```diff
- xor reg, reg
```

</td>
<td align=center>→</td>
<td>

```diff
+ mov reg, 0
```

</td>
                </tr>
            </table>
        </li>
        <li>
            ✅ <b>RandomizeFrameInsertions</b>: Randomizes the function prologue/epilogue insertion.
            <br/>
            <sup>Implemented in <a href="https://github.com/tijme/dittobytes/releases/tag/release-1.0.5">release 1.0.5</a>.</sup>
            <table>
                <tr>
                    <td align=center>Original</td>
                    <td></td>
                    <td align=center>Metamorphicated (example)</td>
                </tr>
                <tr>
                    <td>

```diff
# sub_0
# push    rbp
# mov     rbp, rsp
- push    r11
- push    r15
```

</td>
<td align=center>→</td>
<td>

```diff
# sub_0
# push    rbp
# mov     rbp, rsp
+ push    r15
+ push    r14
```

</td>
                </tr>
            </table>
        </li>
        <li>
            ⏳ Swap simple math (e.g., <code>`sub [reg], imm`</code> → <code>`add [reg], -imm`</code>).
            <br/>
            <sup>Yet to implement.</sup>
        </li>
        <li>
            ⏳ Instruction substitution (e.g., <code>`mov [mem], imm`</code> → <code>`push imm; pop [mem]`</code>)
            <br/>
            <sup>Yet to implement.</sup>
        </li>
        <li>
            ⏳ Insertion of fake basic blocks based on assembly from trusted software.
            <br/>
            <sup>Yet to implement.</sup>
        </li>
    </ul>
    <hr>
</details>

<details>
    <summary>Other To-Dos</summary>
    <hr>
    <ul>
        <li>
            ✅ Implement a more complex and larger feature test to verify correctness of compiled shellcode.
            <br/>
            <sup>Implemented in <a href="https://github.com/tijme/dittobytes/releases/tag/release-1.0.1">release 1.0.1</a>.
        </li>
        <li>
            ✅ Test & report Levenshtein distance of different shellcode compilations.
            <br/>
            <sup>Implemented in <a href="https://github.com/tijme/dittobytes/releases/tag/release-1.0.4">release 1.0.4</a>.
        </li>
        <li>
            ✅  Generate regular executable files alongside the already compiled shellcodes.
            <br/>
            <sup>Implemented in <a href="https://github.com/tijme/dittobytes/releases/tag/release-1.0.7">release 1.0.7</a>.
        </li>
    </ul>
    <hr>
</details>

<h1><img src="https://gist.githubusercontent.com/tijme/c77f321c8dacd6d8ce8e0f9e2ab8c719/raw/029692c26cb1dd1c05f1c4544a96d333544b9f3a/logo-emblem.svg" width=30 height=30 /> Limitations</h1>

There is currently one known limitation in the use of Dittobytes.

* LLVM cannot inline compile `float`'s and `double`'s, causing them to end up in the `.rodata` segment. As a result, these types do not work when compiled with Dittobytes.

<h1><img src="https://gist.githubusercontent.com/tijme/c77f321c8dacd6d8ce8e0f9e2ab8c719/raw/029692c26cb1dd1c05f1c4544a96d333544b9f3a/logo-emblem.svg" width=30 height=30 /> Issues & requests</h1>

Issues or new feature requests can be reported via the [issue tracker](https://github.com/tijme/dittobytes/issues). Please make sure your issue or feature has not yet been reported by anyone else before submitting a new one.

<h1><img src="https://gist.githubusercontent.com/tijme/c77f321c8dacd6d8ce8e0f9e2ab8c719/raw/029692c26cb1dd1c05f1c4544a96d333544b9f3a/logo-emblem.svg" width=30 height=30 /> License & copyright</h1>

Copyright &copy; 2025 Tijme Gommers. Dittobytes is released under the Mozilla Public License Version 2.0. View [LICENSE.md](https://github.com/tijme/dittobytes/blob/master/LICENSE.md) for the full license. Dittobytes depends on various open-source components which all have their own license and copyright.
