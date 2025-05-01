#!/usr/bin/make -f

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

BUILD_DIR              := ../builds
PYTHON_PATH            := /usr/bin/python3
LLVM_DIR_WIN           := /opt/llvm-winlin/bin
LLVM_DIR_LIN           := /opt/llvm-winlin/bin
LLVM_DIR_MAC           := /usr/bin

WIN_AMD64_LOADER_NAME  := loader-win-amd64
WIN_ARM64_LOADER_NAME  := loader-win-arm64
LIN_AMD64_LOADER_NAME  := loader-lin-amd64
LIN_ARM64_LOADER_NAME  := loader-lin-arm64
MAC_AMD64_LOADER_NAME  := loader-mac-amd64
MAC_ARM64_LOADER_NAME  := loader-mac-arm64

IS_COMPILER_CONTAINER  := $(shell if [ "$(IS_COMPILER_CONTAINER)" = "true" ] || [ -f /tmp/.dittobytes-env-loaders ] || [ -f /tmp/.dittobytes-env-all-encompassing ]; then echo "true"; else echo "false"; fi)

##########################################
## Default runs                         ##
##########################################

all: check_environment \
	$(WIN_AMD64_LOADER_NAME) \
	$(WIN_ARM64_LOADER_NAME) \
	$(LIN_AMD64_LOADER_NAME) \
	$(LIN_ARM64_LOADER_NAME) \
	$(MAC_AMD64_LOADER_NAME) \
	$(MAC_ARM64_LOADER_NAME)

##########################################
## Environment check                    ##
##########################################

check_environment:
ifeq ($(IS_COMPILER_CONTAINER), false)
	@echo "[+] It appears you are not running this command inside the \`Dittobytes Loaders Compiler Container\`."
	@echo "[+] You can build it and run in in the root of the Dittobytes project directory."
	@echo "    $ docker buildx build -t dittobytes ."
	@echo "    $ docker run --rm -v ".:/tmp/workdir" -it dittobytes-loaders"
	@read -p "[+] Do you want to continue anyway? (y/N) " CONTINUE && \
	case "$$CONTINUE" in \
		[yY][eE][sS]|[yY]) echo "[+] Continuing outside container..." ;; \
		*) echo "[!] Aborting." && exit 1 ;; \
	esac
	$(eval IS_COMPILER_CONTAINER := true)
endif

##########################################
## Windows AMD64                        ##
##########################################

WIN_AMD64_TARGET         := x86_64-w64-mingw32
WIN_AMD64_DEFINES        := -D__WINDOWS__ -D__AMD64__
WIN_AMD64_LOADER_PATH    := $(BUILD_DIR)/$(WIN_AMD64_LOADER_NAME)
WIN_AMD64_LOADER_FLAGS   := -target $(WIN_AMD64_TARGET) $(WIN_AMD64_DEFINES) -fuse-ld=lld --rtlib=compiler-rt

$(WIN_AMD64_LOADER_PATH).exe: windows/src/main.c | $(BUILD_DIR)
	@echo "[+] Compiling $(WIN_AMD64_LOADER_NAME)."
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) clang $(WIN_AMD64_LOADER_FLAGS) $< -o $@

$(WIN_AMD64_LOADER_NAME): $(WIN_AMD64_LOADER_PATH).exe
	@echo "    - Done building $@."

##########################################
## Windows ARM64                        ##
##########################################

WIN_ARM64_TARGET         := aarch64-w64-mingw32
WIN_ARM64_DEFINES        := -D__WINDOWS__ -D__ARM64__
WIN_ARM64_LOADER_PATH    := $(BUILD_DIR)/$(WIN_ARM64_LOADER_NAME)
WIN_ARM64_LOADER_FLAGS   := -target $(WIN_ARM64_TARGET) $(WIN_ARM64_DEFINES) -fuse-ld=lld --rtlib=compiler-rt

$(WIN_ARM64_LOADER_PATH).exe: windows/src/main.c | $(BUILD_DIR)
	@echo "[+] Compiling $(WIN_ARM64_LOADER_NAME)."
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) clang $(WIN_ARM64_LOADER_FLAGS) $< -o $@

$(WIN_ARM64_LOADER_NAME): $(WIN_ARM64_LOADER_PATH).exe
	@echo "    - Done building $@."

##########################################
## Linux AMD64                          ##
##########################################

LIN_AMD64_TARGET         := x86_64-linux-gnu
LIN_AMD64_DEFINES        := -D__LINUX__ -D__AMD64__
LIN_AMD64_LOADER_PATH    := $(BUILD_DIR)/$(LIN_AMD64_LOADER_NAME)
LIN_AMD64_LOADER_FLAGS   := -target $(LIN_AMD64_TARGET) $(LIN_AMD64_DEFINES) -fuse-ld=lld

$(LIN_AMD64_LOADER_PATH): linux/src/main.c | $(BUILD_DIR)
	@echo "[+] Compiling $(LIN_AMD64_LOADER_NAME)."
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_LIN):$(PATH) clang $(LIN_AMD64_LOADER_FLAGS) $< -o $@

$(LIN_AMD64_LOADER_NAME): $(LIN_AMD64_LOADER_PATH)
	@echo "    - Done building $@."

##########################################
## Linux ARM64                          ##
##########################################

LIN_ARM64_TARGET         := aarch64-linux-gnu
LIN_ARM64_DEFINES        := -D__LINUX__ -D__ARM64__ -DEntryFunction=shellcode
LIN_ARM64_LOADER_PATH    := $(BUILD_DIR)/$(LIN_ARM64_LOADER_NAME)
LIN_ARM64_LOADER_FLAGS   := -target $(LIN_ARM64_TARGET) $(LIN_ARM64_DEFINES) -fuse-ld=lld --sysroot=/

$(LIN_ARM64_LOADER_PATH): linux/src/main.c | $(BUILD_DIR)
	@echo "[+] Compiling $(LIN_ARM64_LOADER_NAME)."
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_LIN):$(PATH) clang $(LIN_ARM64_LOADER_FLAGS) $< -o $@

$(LIN_ARM64_LOADER_NAME): $(LIN_ARM64_LOADER_PATH)
	@echo "    - Done building $@."

##########################################
## MacOS AMD64                          ##
##########################################

MAC_AMD64_TARGET         := x86_64-apple-darwin
MAC_AMD64_DEFINES        := -D__MACOS__ -D__AMD64__
MAC_AMD64_LOADER_PATH    := $(BUILD_DIR)/$(MAC_AMD64_LOADER_NAME)
MAC_AMD64_LOADER_FLAGS   := -target $(MAC_AMD64_TARGET) $(MAC_AMD64_DEFINES) -fuse-ld=lld -isysroot/opt/macos-sdk/MacOSX15.4.sdk/ -I/opt/macos-sdk/MacOSX15.4.sdk/usr/include -L/opt/macos-sdk/MacOSX15.4.sdk/usr/lib

$(MAC_AMD64_LOADER_PATH): macos/src/main.c | $(BUILD_DIR)
	@echo "[+] Compiling $(MAC_AMD64_LOADER_NAME)."
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_MAC):$(PATH) clang $(MAC_AMD64_LOADER_FLAGS) $< -o $@

$(MAC_AMD64_LOADER_NAME): $(MAC_AMD64_LOADER_PATH)
	@echo "    - Done building $@."

##########################################
## MacOS ARM64                          ##
##########################################

MAC_ARM64_TARGET         := arm64-apple-darwin
MAC_ARM64_DEFINES        := -D__MACOS__ -D__ARM64__
MAC_ARM64_LOADER_PATH    := $(BUILD_DIR)/$(MAC_ARM64_LOADER_NAME)
MAC_ARM64_LOADER_FLAGS   := -target $(MAC_ARM64_TARGET) $(MAC_ARM64_DEFINES) -fuse-ld=lld -isysroot/opt/macos-sdk/MacOSX15.4.sdk/ -I/opt/macos-sdk/MacOSX15.4.sdk/usr/include -L/opt/macos-sdk/MacOSX15.4.sdk/usr/lib

$(MAC_ARM64_LOADER_PATH): macos/src/main.c | $(BUILD_DIR)
	@echo "[+] Compiling $(MAC_ARM64_LOADER_NAME)."
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_MAC):$(PATH) clang $(MAC_ARM64_LOADER_FLAGS) $< -o $@

$(MAC_ARM64_LOADER_NAME): $(MAC_ARM64_LOADER_PATH)
	@echo "    - Done building $@."

##########################################
## Utility targets                      ##
##########################################

$(BUILD_DIR):
	@echo "[+] Creating loaders build folder."
	@mkdir -p $(BUILD_DIR)

clean:
	@echo "[+] Removing loaders from build folder."
	@rm -rf $(BUILD_DIR)/loader-*

dependencies:
	@echo "[+] Installing Python dependencies."
	@$(PYTHON_PATH) -m pip install --upgrade pip
	@$(PYTHON_PATH) -m pip install -r scripts/requirements.txt --break-system-packages

list:
	@echo "[+] Available loader targets:"
	@echo "    - $(WIN_AMD64_LOADER_NAME)"
	@echo "    - $(WIN_ARM64_LOADER_NAME)"
	@echo "    - $(LIN_AMD64_LOADER_NAME)"
	@echo "    - $(LIN_ARM64_LOADER_NAME)"
	@echo "    - $(MAC_AMD64_LOADER_NAME)"
	@echo "    - $(MAC_ARM64_LOADER_NAME)"

.PHONY: all dependencies clean \
	$(WIN_AMD64_LOADER_NAME) $(WIN_AMD64_LOADER_PATH) $(WIN_AMD64_LOADER_PATH).exe $(WIN_AMD64_LOADER_PATH).obj $(WIN_AMD64_LOADER_PATH).bin $(WIN_AMD64_LOADER_PATH).lkd \
	$(WIN_ARM64_LOADER_NAME) $(WIN_ARM64_LOADER_PATH) $(WIN_ARM64_LOADER_PATH).exe $(WIN_ARM64_LOADER_PATH).obj $(WIN_ARM64_LOADER_PATH).bin $(WIN_ARM64_LOADER_PATH).lkd \
	$(LIN_AMD64_LOADER_NAME) $(LIN_AMD64_LOADER_PATH) $(LIN_AMD64_LOADER_PATH).exe $(LIN_AMD64_LOADER_PATH).obj $(LIN_AMD64_LOADER_PATH).bin $(LIN_AMD64_LOADER_PATH).lkd \
	$(LIN_ARM64_LOADER_NAME) $(LIN_ARM64_LOADER_PATH) $(LIN_ARM64_LOADER_PATH).exe $(LIN_ARM64_LOADER_PATH).obj $(LIN_ARM64_LOADER_PATH).bin $(LIN_ARM64_LOADER_PATH).lkd \
	$(MAC_AMD64_LOADER_NAME) $(MAC_AMD64_LOADER_PATH) $(MAC_AMD64_LOADER_PATH).exe $(MAC_AMD64_LOADER_PATH).obj $(MAC_AMD64_LOADER_PATH).bin $(MAC_AMD64_LOADER_PATH).lkd \
	$(MAC_ARM64_LOADER_NAME) $(MAC_ARM64_LOADER_PATH) $(MAC_ARM64_LOADER_PATH).exe $(MAC_ARM64_LOADER_PATH).obj $(MAC_ARM64_LOADER_PATH).bin $(MAC_ARM64_LOADER_PATH).lkd
