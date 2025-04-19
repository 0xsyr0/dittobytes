# GNU General Public License, version 2.0.
#
# Copyright (c) 2025 Tijme Gommers (@tijme).
#
# This source code file is part of Dittobytes. Dittobytes is 
# licensed under GNU General Public License, version 2.0, and 
# you are free to use, modify, and distribute this file under 
# its terms. However, any modified versions of this file must 
# include this same license and copyright notice.

##########################################
## Globals                              ##
##########################################

BUILD_DIR              := builds
PYTHON_PATH            := /usr/bin/python3
LLVM_DIR_WIN           := /opt/llvm-winlin/bin
LLVM_DIR_LIN           := /opt/llvm-winlin/bin
LLVM_DIR_MAC           := /usr/bin

WIN_AMD64_BEACON_NAME  := beacon-windows-x64
WIN_ARM64_BEACON_NAME  := beacon-windows-arm64
LIN_AMD64_BEACON_NAME  := beacon-linux-x64
LIN_ARM64_BEACON_NAME  := beacon-linux-arm64
MAC_AMD64_BEACON_NAME  := beacon-macos-x64
MAC_ARM64_BEACON_NAME  := beacon-macos-arm64

IS_DOCKER              := $(shell test -f /.dittobytes-beacons-env && echo "true" || echo "false")

##########################################
## Default runs                         ##
##########################################
all: check_environment \
	$(WIN_AMD64_BEACON_NAME) \
	$(WIN_ARM64_BEACON_NAME) \
	$(LIN_AMD64_BEACON_NAME) \
	$(LIN_ARM64_BEACON_NAME) \
	$(MAC_AMD64_BEACON_NAME) \
	$(MAC_ARM64_BEACON_NAME)

##########################################
## Environment check                    ##
##########################################
check_environment:
ifeq ($(IS_DOCKER), false)
	@echo "[+] It appears you are not running this command inside the \`Dittobytes Beacon Docker Compiler\`."
	@echo "[+] You can build it and run in in the root of the Dittobytes project directory."
	@echo "    $ docker build -t dittobytes ."
	@echo "    $ docker run --rm -v ".:/tmp/workdir" -it dittobytes"
	@read -p "[+] Do you want to continue anyway? (y/N) " CONTINUE && \
	case "$$CONTINUE" in \
		[yY][eE][sS]|[yY]) echo "[+] Continuing outside Docker..." ;; \
		*) echo "[!] Aborting." && exit 1 ;; \
	esac
endif

##########################################
## Windows AMD64                        ##
##########################################

WIN_AMD64_TARGET            := x86_64-w64-mingw32
WIN_AMD64_DEFINES           := -D__WINDOWS__ -D__AMD64__ -DEntryFunction=shellcode
WIN_AMD64_BEACON_PATH       := $(BUILD_DIR)/$(WIN_AMD64_BEACON_NAME)
WIN_AMD64_BEACON_CL1FLAGS   := -target $(WIN_AMD64_TARGET) $(WIN_AMD64_DEFINES) -fuse-ld=lld -O0 -emit-llvm -S -fPIC -ffreestanding -nostdlib -nodefaultlibs -fpass-plugin=./transpilers/intermediate/build/libIntermediateTranspiler.so -Xclang -disable-O0-optnone -fPIC -fno-rtti -fno-exceptions -fno-delayed-template-parsing -fno-modules -fno-fast-math -fno-builtin -fno-elide-constructors -fno-access-control -fno-jump-tables -fno-omit-frame-pointer -fno-ident
WIN_AMD64_BEACON_LLCFLAGS   := -mtriple $(WIN_AMD64_TARGET) -march=x86-64 --relocation-model=pic
WIN_AMD64_BEACON_CL2FLAGS   := -target $(WIN_AMD64_TARGET) $(WIN_AMD64_DEFINES) -fuse-ld=lld -e shellcode -fPIC -ffreestanding -nostdlib -nodefaultlibs  

$(WIN_AMD64_BEACON_PATH).ll: beacon/main.c | $(BUILD_DIR)
	@echo "[+] Compiling $(WIN_AMD64_BEACON_NAME)."
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) clang $(WIN_AMD64_BEACON_CL1FLAGS) $< -o $@

$(WIN_AMD64_BEACON_PATH).mir: $(WIN_AMD64_BEACON_PATH).ll
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) llc $(WIN_AMD64_BEACON_LLCFLAGS) -stop-after=virtregrewriter -o $@ $<

$(WIN_AMD64_BEACON_PATH).poly.mir: $(WIN_AMD64_BEACON_PATH).mir
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) llc $(WIN_AMD64_BEACON_LLCFLAGS) -load ./transpilers/machine/build/libMachineTranspiler.so --run-pass=MachineTranspiler -o $@ $<

$(WIN_AMD64_BEACON_PATH).obj: $(WIN_AMD64_BEACON_PATH).poly.mir
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) llc $(WIN_AMD64_BEACON_LLCFLAGS) -filetype=obj -o $@ $<

$(WIN_AMD64_BEACON_PATH).lkd: $(WIN_AMD64_BEACON_PATH).obj
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) clang $(WIN_AMD64_BEACON_CL2FLAGS) -o $@ $<

$(WIN_AMD64_BEACON_PATH).bin: $(WIN_AMD64_BEACON_PATH).lkd
	@echo "    - Intermediate compile of $@."
	@$(PYTHON_PATH) ./scripts/extract-text-segment.py $< $@

$(WIN_AMD64_BEACON_NAME): $(WIN_AMD64_BEACON_PATH).bin
	@echo "    - Intermediate cleanup of build files."
	@rm $(WIN_AMD64_BEACON_PATH).lkd $(WIN_AMD64_BEACON_PATH).obj $(WIN_AMD64_BEACON_PATH).*mir $(WIN_AMD64_BEACON_PATH).ll
	@echo "    - Done building $@."

##########################################
## Windows ARM64                        ##
##########################################

WIN_ARM64_TARGET         := aarch64-w64-mingw32
WIN_ARM64_DEFINES        := -D__WINDOWS__ -D__ARM64__ -DEntryFunction=shellcode
WIN_ARM64_BEACON_PATH    := $(BUILD_DIR)/$(WIN_ARM64_BEACON_NAME)
WIN_ARM64_BEACON_CFLAGS  := -target $(WIN_ARM64_TARGET) $(WIN_ARM64_DEFINES) UNDEFINED -c -fpass-plugin=./transpiler/build/libMachineTranspiler.so -Xclang -disable-O0-optnone -fPIC -fno-rtti -fno-exceptions -fno-delayed-template-parsing -fno-modules -fno-fast-math -fno-builtin -fno-elide-constructors -fno-access-control -fno-jump-tables -fno-omit-frame-pointer -fno-ident
WIN_ARM64_BEACON_LDFLAGS := -target $(WIN_ARM64_TARGET) $(WIN_ARM64_DEFINES) -fuse-ld=lld -nostdlib -e shellcode

$(WIN_ARM64_BEACON_PATH).obj: beacon/main.c | $(BUILD_DIR)
	@echo "[+] Compiling $(WIN_ARM64_BEACON_NAME)."
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) clang $(WIN_ARM64_BEACON_CFLAGS) $< -o $@

$(WIN_ARM64_BEACON_PATH).lkd: $(WIN_ARM64_BEACON_PATH).obj
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) clang $(WIN_ARM64_BEACON_LDFLAGS) -o $@ $<

$(WIN_ARM64_BEACON_PATH).bin: $(WIN_ARM64_BEACON_PATH).lkd
	@echo "    - Intermediate compile of $@."
	@$(PYTHON_PATH) ./scripts/extract-text-segment.py $< $@

$(WIN_ARM64_BEACON_NAME): $(WIN_ARM64_BEACON_PATH).bin
	@echo "    - Intermediate cleanup of build files."
	@rm $(WIN_ARM64_BEACON_PATH).lkd $(WIN_ARM64_BEACON_PATH).obj
	@echo "    - Done building $@."

##########################################
## Linux AMD64                          ##
##########################################

LIN_AMD64_TARGET            := x86_64-linux-gnu
LIN_AMD64_DEFINES           := -D__LINUX__ -D__AMD64__ -DEntryFunction=shellcode
LIN_AMD64_BEACON_PATH       := $(BUILD_DIR)/$(LIN_AMD64_BEACON_NAME)
LIN_AMD64_BEACON_CL1FLAGS   := -target $(LIN_AMD64_TARGET) $(LIN_AMD64_DEFINES) -O0 -emit-llvm -S -fPIC -ffreestanding -nostdlib -nodefaultlibs -fpass-plugin=./transpilers/intermediate/build/libIntermediateTranspiler.so -Xclang -disable-O0-optnone -fPIC -fno-rtti -fno-exceptions -fno-delayed-template-parsing -fno-modules -fno-fast-math -fno-builtin -fno-elide-constructors -fno-access-control -fno-jump-tables -fno-omit-frame-pointer -fno-ident
LIN_AMD64_BEACON_LLCFLAGS   := -mtriple $(LIN_AMD64_TARGET) -march=x86-64 --relocation-model=pic
LIN_AMD64_BEACON_CL2FLAGS   := -target $(LIN_AMD64_TARGET) $(LIN_AMD64_DEFINES) -fuse-ld=lld -e shellcode -fPIC -ffreestanding -nostdlib -nodefaultlibs  

$(LIN_AMD64_BEACON_PATH).ll: beacon/main.c | $(BUILD_DIR)
	@echo "[+] Compiling $(LIN_AMD64_BEACON_NAME)."
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_LIN):$(PATH) clang $(LIN_AMD64_BEACON_CL1FLAGS) $< -o $@

$(LIN_AMD64_BEACON_PATH).mir: $(LIN_AMD64_BEACON_PATH).ll
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_LIN):$(PATH) llc $(LIN_AMD64_BEACON_LLCFLAGS) -stop-after=virtregrewriter -o $@ $<

$(LIN_AMD64_BEACON_PATH).poly.mir: $(LIN_AMD64_BEACON_PATH).mir
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_LIN):$(PATH) llc $(LIN_AMD64_BEACON_LLCFLAGS) -load ./transpilers/machine/build/libMachineTranspiler.so --run-pass=MachineTranspiler -o $@ $<

$(LIN_AMD64_BEACON_PATH).obj: $(LIN_AMD64_BEACON_PATH).poly.mir
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_LIN):$(PATH) llc $(LIN_AMD64_BEACON_LLCFLAGS) -filetype=obj -o $@ $<

$(LIN_AMD64_BEACON_PATH).lkd: $(LIN_AMD64_BEACON_PATH).obj
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_LIN):$(PATH) clang $(LIN_AMD64_BEACON_CL2FLAGS) -o $@ $<

$(LIN_AMD64_BEACON_PATH).bin: $(LIN_AMD64_BEACON_PATH).lkd
	@echo "    - Intermediate compile of $@."
	@$(PYTHON_PATH) ./scripts/extract-text-segment.py $< $@

$(LIN_AMD64_BEACON_NAME): $(LIN_AMD64_BEACON_PATH).bin
	@echo "    - Intermediate cleanup of build files."
	@rm $(LIN_AMD64_BEACON_PATH).lkd $(LIN_AMD64_BEACON_PATH).obj $(LIN_AMD64_BEACON_PATH).*mir $(LIN_AMD64_BEACON_PATH).ll
	@echo "    - Done building $@."

##########################################
## Linux ARM64                          ##
##########################################

LIN_ARM64_TARGET         := aarch64-linux-gnu
LIN_ARM64_DEFINES        := -D__LINUX__ -D__ARM64__ -DEntryFunction=shellcode
LIN_ARM64_BEACON_PATH    := $(BUILD_DIR)/$(LIN_ARM64_BEACON_NAME)
LIN_ARM64_BEACON_CFLAGS  := -target $(LIN_ARM64_TARGET) $(LIN_ARM64_DEFINES) UNDEFINED -c -fpass-plugin=./transpiler/build/libMachineTranspiler.so -Xclang -disable-O0-optnone -fPIC -fno-rtti -fno-exceptions -fno-delayed-template-parsing -fno-modules -fno-fast-math -fno-builtin -fno-elide-constructors -fno-access-control -fno-jump-tables -fno-omit-frame-pointer -fno-ident
LIN_ARM64_BEACON_LDFLAGS := -target $(LIN_ARM64_TARGET) $(LIN_ARM64_DEFINES) -fuse-ld=lld -nostdlib -e shellcode

$(LIN_ARM64_BEACON_PATH).obj: beacon/main.c | $(BUILD_DIR)
	@echo "[+] Compiling $(LIN_ARM64_BEACON_NAME)."
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) clang $(LIN_ARM64_BEACON_CFLAGS) $< -o $@

$(LIN_ARM64_BEACON_PATH).lkd: $(LIN_ARM64_BEACON_PATH).obj
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) clang $(LIN_ARM64_BEACON_LDFLAGS) -o $@ $<

$(LIN_ARM64_BEACON_PATH).bin: $(LIN_ARM64_BEACON_PATH).lkd
	@echo "    - Intermediate compile of $@."
	@$(PYTHON_PATH) ./scripts/extract-text-segment.py $< $@

$(LIN_ARM64_BEACON_NAME): $(LIN_ARM64_BEACON_PATH).bin
	@echo "    - Intermediate cleanup of build files."
	@rm $(LIN_ARM64_BEACON_PATH).lkd $(LIN_ARM64_BEACON_PATH).obj
	@echo "    - Done building $@."

##########################################
## MacOS AMD64                          ##
##########################################

MAC_AMD64_TARGET            := x86_64-apple-darwin
MAC_AMD64_DEFINES           := -D__MACOS__ -D__AMD64__ -DEntryFunction=main
MAC_AMD64_BEACON_PATH       := $(BUILD_DIR)/$(MAC_AMD64_BEACON_NAME)
MAC_AMD64_BEACON_CL1FLAGS   := -target $(MAC_AMD64_TARGET) $(MAC_AMD64_DEFINES) -O0 -emit-llvm -S -fPIC -ffreestanding -nostdlib -nodefaultlibs -isysroot/opt/macos-sdk/MacOSX15.4.sdk/ -I/opt/macos-sdk/MacOSX15.4.sdk/usr/include -fpass-plugin=./transpilers/intermediate/build/libIntermediateTranspiler.so -Xclang -disable-O0-optnone -fPIC -fno-rtti -fno-exceptions -fno-delayed-template-parsing -fno-modules -fno-fast-math -fno-builtin -fno-elide-constructors -fno-access-control -fno-jump-tables -fno-omit-frame-pointer -fno-ident
MAC_AMD64_BEACON_LLCFLAGS   := -mtriple $(MAC_AMD64_TARGET) -march=x86-64 --relocation-model=pic
MAC_AMD64_BEACON_CL2FLAGS   := -target $(MAC_AMD64_TARGET) $(MAC_AMD64_DEFINES) -fuse-ld=lld -fPIC -ffreestanding -nostdlib -nodefaultlibs -isysroot/opt/macos-sdk/MacOSX15.4.sdk/ -L/opt/macos-sdk/MacOSX15.4.sdk/usr/lib

$(MAC_AMD64_BEACON_PATH).ll: beacon/main.c | $(BUILD_DIR)
	@echo "[+] Compiling $(MAC_AMD64_BEACON_NAME)."
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_MAC):$(PATH) clang $(MAC_AMD64_BEACON_CL1FLAGS) $< -o $@

$(MAC_AMD64_BEACON_PATH).mir: $(MAC_AMD64_BEACON_PATH).ll
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_MAC):$(PATH) llc $(MAC_AMD64_BEACON_LLCFLAGS) -stop-after=virtregrewriter -o $@ $<

$(MAC_AMD64_BEACON_PATH).poly.mir: $(MAC_AMD64_BEACON_PATH).mir
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_MAC):$(PATH) llc $(MAC_AMD64_BEACON_LLCFLAGS) -load ./transpilers/machine/build/libMachineTranspiler.so --run-pass=MachineTranspiler -o $@ $<

$(MAC_AMD64_BEACON_PATH).obj: $(MAC_AMD64_BEACON_PATH).poly.mir
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_MAC):$(PATH) llc $(MAC_AMD64_BEACON_LLCFLAGS) -filetype=obj -o $@ $<

$(MAC_AMD64_BEACON_PATH).lkd: $(MAC_AMD64_BEACON_PATH).obj
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_MAC):$(PATH) clang $(MAC_AMD64_BEACON_CL2FLAGS) -o $@ $<

$(MAC_AMD64_BEACON_PATH).bin: $(MAC_AMD64_BEACON_PATH).lkd
	@echo "    - Intermediate compile of $@."
	@$(PYTHON_PATH) ./scripts/extract-text-segment.py $< $@

$(MAC_AMD64_BEACON_NAME): $(MAC_AMD64_BEACON_PATH).bin
	@echo "    - Intermediate cleanup of build files."
	@rm $(MAC_AMD64_BEACON_PATH).lkd $(MAC_AMD64_BEACON_PATH).obj $(MAC_AMD64_BEACON_PATH).*mir $(MAC_AMD64_BEACON_PATH).ll
	@echo "    - Done building $@."

##########################################
## MacOS ARM64                          ##
##########################################

MAC_ARM64_TARGET         := arm64-apple-darwin
MAC_ARM64_DEFINES        := -D__MACOS__ -D__ARM64__ -DEntryFunction=main
MAC_ARM64_BEACON_PATH    := $(BUILD_DIR)/$(MAC_ARM64_BEACON_NAME)
MAC_ARM64_BEACON_CFLAGS  := -target $(MAC_ARM64_TARGET) $(MAC_ARM64_DEFINES) UNDEFINED -c -isysroot/opt/macos-sdk/MacOSX15.4.sdk/ -I/opt/macos-sdk/MacOSX15.4.sdk/usr/include -fpass-plugin=./transpiler/build/libMachineTranspiler.so -Xclang -disable-O0-optnone -fPIC -fno-rtti -fno-exceptions -fno-delayed-template-parsing -fno-modules -fno-fast-math -fno-builtin -fno-elide-constructors -fno-access-control -fno-jump-tables -fno-omit-frame-pointer -fno-ident
MAC_ARM64_BEACON_LDFLAGS := -target $(MAC_ARM64_TARGET) $(MAC_ARM64_DEFINES) -fuse-ld=lld -nostdlib -isysroot/opt/macos-sdk/MacOSX15.4.sdk/ -L/opt/macos-sdk/MacOSX15.4.sdk/usr/lib

$(MAC_ARM64_BEACON_PATH).obj: beacon/main.c | $(BUILD_DIR)
	@echo "[+] Compiling $(MAC_ARM64_BEACON_NAME)."
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_MAC):$(PATH) clang $(MAC_ARM64_BEACON_CFLAGS) $< -o $@

$(MAC_ARM64_BEACON_PATH).lkd: $(MAC_ARM64_BEACON_PATH).obj
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_MAC):$(PATH) clang $(MAC_ARM64_BEACON_LDFLAGS) -o $@ $<

$(MAC_ARM64_BEACON_PATH).bin: $(MAC_ARM64_BEACON_PATH).lkd
	@echo "    - Intermediate compile of $@."
	@$(PYTHON_PATH) ./scripts/extract-text-segment.py $< $@

$(MAC_ARM64_BEACON_NAME): $(MAC_ARM64_BEACON_PATH).bin
	@echo "    - Intermediate cleanup of build files."
	@rm $(MAC_ARM64_BEACON_PATH).lkd $(MAC_ARM64_BEACON_PATH).obj
	@echo "    - Done building $@."

##########################################
## Utility targets                      ##
##########################################

# Windows x86_64 beacon
$(BUILD_DIR):
	@echo "[+] Creating build directory"
	@mkdir -p $(BUILD_DIR)

dependencies:
	@echo "[+] Installing Python dependencies"
	@$(PYTHON_PATH) -m pip install --upgrade pip
	@$(PYTHON_PATH) -m pip install -r scripts/requirements.txt --break-system-packages

clean:
	@echo "[+] Cleaning build folder"
	@rm -f $(BUILD_DIR)/beacon-*

list:
	@echo "[+] Available beacon targets:"
	@echo "    - $(WIN_AMD64_BEACON_NAME)"
	@echo "    - $(WIN_ARM64_BEACON_NAME)"
	@echo "    - $(LIN_AMD64_BEACON_NAME)"
	@echo "    - $(LIN_ARM64_BEACON_NAME)"
	@echo "    - $(MAC_AMD64_BEACON_NAME)"
	@echo "    - $(MAC_ARM64_BEACON_NAME)"

.PHONY: all dependencies clean \
	$(WIN_AMD64_BEACON_NAME) $(WIN_AMD64_BEACON_PATH) $(WIN_AMD64_BEACON_PATH).exe $(WIN_AMD64_BEACON_PATH).obj $(WIN_AMD64_BEACON_PATH).bin $(WIN_AMD64_BEACON_PATH).lkd $(WIN_AMD64_BEACON_PATH).ll $(WIN_AMD64_BEACON_PATH).poly.mir $(WIN_AMD64_BEACON_PATH).mir \
	$(WIN_ARM64_BEACON_NAME) $(WIN_ARM64_BEACON_PATH) $(WIN_ARM64_BEACON_PATH).exe $(WIN_ARM64_BEACON_PATH).obj $(WIN_ARM64_BEACON_PATH).bin $(WIN_ARM64_BEACON_PATH).lkd $(WIN_ARM64_BEACON_PATH).ll $(WIN_ARM64_BEACON_PATH).poly.mir $(WIN_ARM64_BEACON_PATH).mir \
	$(LIN_AMD64_BEACON_NAME) $(LIN_AMD64_BEACON_PATH) $(LIN_AMD64_BEACON_PATH).exe $(LIN_AMD64_BEACON_PATH).obj $(LIN_AMD64_BEACON_PATH).bin $(LIN_AMD64_BEACON_PATH).lkd $(LIN_AMD64_BEACON_PATH).ll $(LIN_AMD64_BEACON_PATH).poly.mir $(LIN_AMD64_BEACON_PATH).mir \
	$(LIN_ARM64_BEACON_NAME) $(LIN_ARM64_BEACON_PATH) $(LIN_ARM64_BEACON_PATH).exe $(LIN_ARM64_BEACON_PATH).obj $(LIN_ARM64_BEACON_PATH).bin $(LIN_ARM64_BEACON_PATH).lkd $(LIN_ARM64_BEACON_PATH).ll $(LIN_ARM64_BEACON_PATH).poly.mir $(LIN_ARM64_BEACON_PATH).mir \
	$(MAC_AMD64_BEACON_NAME) $(MAC_AMD64_BEACON_PATH) $(MAC_AMD64_BEACON_PATH).exe $(MAC_AMD64_BEACON_PATH).obj $(MAC_AMD64_BEACON_PATH).bin $(MAC_AMD64_BEACON_PATH).lkd $(MAC_AMD64_BEACON_PATH).ll $(MAC_AMD64_BEACON_PATH).poly.mir $(MAC_AMD64_BEACON_PATH).mir \
	$(MAC_ARM64_BEACON_NAME) $(MAC_ARM64_BEACON_PATH) $(MAC_ARM64_BEACON_PATH).exe $(MAC_ARM64_BEACON_PATH).obj $(MAC_ARM64_BEACON_PATH).bin $(MAC_ARM64_BEACON_PATH).lkd $(MAC_ARM64_BEACON_PATH).ll $(MAC_ARM64_BEACON_PATH).poly.mir $(MAC_ARM64_BEACON_PATH).mir
