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

DEBUG                                := false
BUILD_DIR                            := ./builds
TESTS_DIR                            := ./tests
SOURCE_PATH                          ?= ./beacon/main.c
PYTHON_PATH                          := python3
LLVM_DIR_WIN                         := /opt/llvm-winlin/bin
LLVM_DIR_LIN                         := /opt/llvm-winlin/bin
LLVM_DIR_MAC                         := /usr/bin

WIN_AMD64_BEACON_NAME                := beacon-win-amd64
WIN_ARM64_BEACON_NAME                := beacon-win-arm64
LIN_AMD64_BEACON_NAME                := beacon-lin-amd64
LIN_ARM64_BEACON_NAME                := beacon-lin-arm64
MAC_AMD64_BEACON_NAME                := beacon-mac-amd64
MAC_ARM64_BEACON_NAME                := beacon-mac-arm64

IS_COMPILER_CONTAINER  := $(shell if [ "$(IS_COMPILER_CONTAINER)" = "true" ] || [ -f /tmp/.dittobytes-env-all-encompassing ]; then echo "true"; else echo "false"; fi)

##########################################
## Metamorphications                    ##
##########################################

MM_DEFAULT						     ?= true

MM_MODIFY_MOV_IMMEDIATE              ?= $(MM_DEFAULT)
MM_TEST_MODIFY_MOV_IMMEDIATE         ?= $(MM_MODIFY_MOV_IMMEDIATE)

MM_MODIFY_XOR_REG_REG                ?= $(MM_DEFAULT)
MM_TEST_MODIFY_XOR_REG_REG           ?= $(MM_MODIFY_XOR_REG_REG)

MM_RANDOM_REGISTER_ALLOCATION        ?= $(MM_DEFAULT)
MM_TEST_RANDOM_REGISTER_ALLOCATION   ?= $(MM_RANDOM_REGISTER_ALLOCATION)

##########################################
## Platform & architecture              ##
##########################################

ifeq ($(OS),Windows_NT)
	CURRENT_PLATFORM := win
	PYTHON_PATH      := python
	ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
		CURRENT_ARCHITECTURE := amd64
	else ifeq ($(PROCESSOR_ARCHITECTURE),ARM64)
		CURRENT_ARCHITECTURE := arm64
	else
		$(error "Dittobytes does not work on Windows-based platforms with architecture: $(UNAME_M)")
	endif
else
	UNAME_S := $(shell uname -s)
	UNAME_M := $(shell uname -m)
	ifeq ($(UNAME_S),Linux)
		CURRENT_PLATFORM := lin
	else ifeq ($(UNAME_S),Darwin)
		CURRENT_PLATFORM := mac
	else
		$(error "Dittobytes does not recognize platform: $(UNAME_S)")
	endif
	ifeq ($(UNAME_M),x86_64)
		CURRENT_ARCHITECTURE := amd64
	else ifeq ($(filter $(UNAME_M),aarch64 arm64),$(UNAME_M))
		CURRENT_ARCHITECTURE := arm64
	else
		$(error "Dittobytes does not work on Unix-based platforms with architecture: $(UNAME_M)")
	endif
endif

##########################################
## Default runs                         ##
##########################################

# By default compile all beacons
all: check_environment beacons

# Alias for all beacons
beacons: check_environment beacon-all-all

# All beacons
beacon-all-all: check_environment \
	$(WIN_AMD64_BEACON_NAME) \
	$(WIN_ARM64_BEACON_NAME) \
	$(LIN_AMD64_BEACON_NAME) \
	$(LIN_ARM64_BEACON_NAME) \
	$(MAC_AMD64_BEACON_NAME) \
	$(MAC_ARM64_BEACON_NAME)

# Platform specific beacons
beacon-win-all: check_environment $(WIN_AMD64_BEACON_NAME) $(WIN_ARM64_BEACON_NAME)
beacon-lin-all: check_environment $(LIN_AMD64_BEACON_NAME) $(LIN_ARM64_BEACON_NAME)
beacon-mac-all: check_environment $(MAC_AMD64_BEACON_NAME) $(MAC_ARM64_BEACON_NAME)

# Architecture specific beacons
beacon-all-amd64: check_environment $(WIN_AMD64_BEACON_NAME) $(LIN_AMD64_BEACON_NAME) $(MAC_AMD64_BEACON_NAME)
beacon-all-arm64: check_environment $(WIN_ARM64_BEACON_NAME) $(LIN_ARM64_BEACON_NAME) $(MAC_ARM64_BEACON_NAME)

# Loaders
loaders: check_environment 
	@echo "[+] Calling \`all\` in loaders makefile."
	@$(MAKE) IS_COMPILER_CONTAINER=$(IS_COMPILER_CONTAINER) --no-print-directory -C ./loaders/

# Transpilers
transpilers: check_environment
	@echo "[+] Calling \`all\` in intermediate transpiler makefile."
	@$(MAKE) IS_COMPILER_CONTAINER=$(IS_COMPILER_CONTAINER) --no-print-directory -C ./transpilers/intermediate/
	@echo "[+] Calling \`all\` in machine transpiler makefile."
	@$(MAKE) IS_COMPILER_CONTAINER=$(IS_COMPILER_CONTAINER) --no-print-directory -C ./transpilers/machine/

# Everything
extensive: check_environment transpilers loaders beacons

##########################################
## Test suite                           ##
##########################################

test-suite-build: check_environment
	@set -e; \
	for AVAILABLE_TEST_OS in $(TESTS_DIR)/*; do \
		for AVAILABLE_TEST_ARCH in $$AVAILABLE_TEST_OS/*; do \
			for TEST_FILE in $$AVAILABLE_TEST_ARCH/*; do \
				[ -f "$$TEST_FILE" ] || continue; \
				AVAILABLE_TEST_OS_BASENAME=$$(basename "$$AVAILABLE_TEST_OS"); \
				AVAILABLE_TEST_ARCH_BASENAME=$$(basename "$$AVAILABLE_TEST_ARCH"); \
				AVAILABLE_TEST_FILE_BASENAME=$$(basename "$$TEST_FILE"); \
				[ -n "$(TEST_SOURCE_PATH)" ] && [ "$$(realpath "$$TEST_FILE")" != "$$(realpath $(TEST_SOURCE_PATH))" ] && continue; \
				if [ -n "$(TEST_OS)" ]; then BEACON_OS="$(TEST_OS)"; else BEACON_OS="$$AVAILABLE_TEST_OS_BASENAME"; fi; \
				if [ -n "$(TEST_ARCH)" ]; then BEACON_ARCH="$(TEST_ARCH)"; else BEACON_ARCH="$$AVAILABLE_TEST_ARCH_BASENAME"; fi; \
				echo "[+] TestSuite building \`$$AVAILABLE_TEST_FILE_BASENAME\` for \`$$BEACON_OS-$$BEACON_ARCH\`."; \
				$(MAKE) IS_COMPILER_CONTAINER=$(IS_COMPILER_CONTAINER) SOURCE_PATH="$$TEST_FILE" BEACON_NAME=$(basename $(notdir $(TESTS_DIR)))_$${AVAILABLE_TEST_OS_BASENAME}_$${AVAILABLE_TEST_ARCH_BASENAME}_$${AVAILABLE_TEST_FILE_BASENAME%.*}_original MM_DEFAULT=false --no-print-directory beacon-$$BEACON_OS-$$BEACON_ARCH; \
				$(MAKE) IS_COMPILER_CONTAINER=$(IS_COMPILER_CONTAINER) SOURCE_PATH="$$TEST_FILE" BEACON_NAME=$(basename $(notdir $(TESTS_DIR)))_$${AVAILABLE_TEST_OS_BASENAME}_$${AVAILABLE_TEST_ARCH_BASENAME}_$${AVAILABLE_TEST_FILE_BASENAME%.*}_transpiled_1 MM_DEFAULT=true --no-print-directory beacon-$$BEACON_OS-$$BEACON_ARCH; \
				$(MAKE) IS_COMPILER_CONTAINER=$(IS_COMPILER_CONTAINER) SOURCE_PATH="$$TEST_FILE" BEACON_NAME=$(basename $(notdir $(TESTS_DIR)))_$${AVAILABLE_TEST_OS_BASENAME}_$${AVAILABLE_TEST_ARCH_BASENAME}_$${AVAILABLE_TEST_FILE_BASENAME%.*}_transpiled_2 MM_DEFAULT=true --no-print-directory beacon-$$BEACON_OS-$$BEACON_ARCH; \
				$(MAKE) IS_COMPILER_CONTAINER=$(IS_COMPILER_CONTAINER) SOURCE_PATH="$$TEST_FILE" BEACON_NAME=$(basename $(notdir $(TESTS_DIR)))_$${AVAILABLE_TEST_OS_BASENAME}_$${AVAILABLE_TEST_ARCH_BASENAME}_$${AVAILABLE_TEST_FILE_BASENAME%.*}_modify_mov_immediate MM_DEFAULT=false MM_MODIFY_MOV_IMMEDIATE=true MM_TEST_MODIFY_MOV_IMMEDIATE=true --no-print-directory beacon-$$BEACON_OS-$$BEACON_ARCH; \
				$(MAKE) IS_COMPILER_CONTAINER=$(IS_COMPILER_CONTAINER) SOURCE_PATH="$$TEST_FILE" BEACON_NAME=$(basename $(notdir $(TESTS_DIR)))_$${AVAILABLE_TEST_OS_BASENAME}_$${AVAILABLE_TEST_ARCH_BASENAME}_$${AVAILABLE_TEST_FILE_BASENAME%.*}_modify_xor_reg_reg MM_DEFAULT=false MM_MODIFY_XOR_REG_REG=true MM_TEST_MODIFY_XOR_REG_REG=true --no-print-directory beacon-$$BEACON_OS-$$BEACON_ARCH; \
				$(MAKE) IS_COMPILER_CONTAINER=$(IS_COMPILER_CONTAINER) SOURCE_PATH="$$TEST_FILE" BEACON_NAME=$(basename $(notdir $(TESTS_DIR)))_$${AVAILABLE_TEST_OS_BASENAME}_$${AVAILABLE_TEST_ARCH_BASENAME}_$${AVAILABLE_TEST_FILE_BASENAME%.*}_random_register_allocation MM_DEFAULT=false MM_RANDOM_REGISTER_ALLOCATION=true MM_TEST_RANDOM_REGISTER_ALLOCATION=true --no-print-directory beacon-$$BEACON_OS-$$BEACON_ARCH; \
			done \
		done \
	done

test-suite-test: check_environment
	@set -e; \
	for AVAILABLE_TEST_OS in $(TESTS_DIR)/*; do \
		for AVAILABLE_TEST_ARCH in $$AVAILABLE_TEST_OS/*; do \
			for TEST_FILE in $$AVAILABLE_TEST_ARCH/*; do \
				[ -f "$$TEST_FILE" ] || continue; \
				AVAILABLE_TEST_OS_BASENAME=$$(basename "$$AVAILABLE_TEST_OS"); \
				AVAILABLE_TEST_ARCH_BASENAME=$$(basename "$$AVAILABLE_TEST_ARCH"); \
				AVAILABLE_TEST_FILE_BASENAME=$$(basename "$$TEST_FILE"); \
				[ -n "$(TEST_SOURCE_PATH)" ] && [ "$$(realpath "$$TEST_FILE")" != "$$(realpath $(TEST_SOURCE_PATH))" ] && continue; \
				if [ -n "$(TEST_OS)" ]; then BEACON_OS="$(TEST_OS)"; else BEACON_OS="$${AVAILABLE_TEST_OS_BASENAME}"; fi; \
				if [ -n "$(TEST_ARCH)" ]; then BEACON_ARCH="$(TEST_ARCH)"; else BEACON_ARCH="$${AVAILABLE_TEST_ARCH_BASENAME}"; fi; \
				echo "[+] TestSuite testing \`$$AVAILABLE_TEST_FILE_BASENAME\` for \`$$BEACON_OS-$$BEACON_ARCH\`."; \
				$(PYTHON_PATH) ./scripts/tests/test.py $${AVAILABLE_TEST_OS_BASENAME} $${AVAILABLE_TEST_ARCH_BASENAME} $${AVAILABLE_TEST_FILE_BASENAME%.*} $$TEST_FILE $$BEACON_OS $$BEACON_ARCH; \
			done; \
		done; \
	done; \
	echo "[+] TestSuite finished successfully!";

test: test-suite-build test-suite-test

##########################################
## Environment check                    ##
##########################################

check_environment:
	@echo "[+] Current makefile configuration for platform \`$(CURRENT_PLATFORM)\` and architecture \`$(CURRENT_ARCHITECTURE)\`:"
	@echo "    MM_MODIFY_MOV_IMMEDIATE:         \`$(MM_MODIFY_MOV_IMMEDIATE)\` (testing: \`$(MM_TEST_MODIFY_MOV_IMMEDIATE)\`)"
	@echo "    MM_MODIFY_XOR_REG_REG:           \`$(MM_MODIFY_XOR_REG_REG)\` (testing: \`$(MM_TEST_MODIFY_XOR_REG_REG)\`)"
	@echo "    MM_RANDOM_REGISTER_ALLOCATION:   \`$(MM_RANDOM_REGISTER_ALLOCATION)\` (testing: \`$(MM_TEST_RANDOM_REGISTER_ALLOCATION)\`)"
ifeq ($(IS_COMPILER_CONTAINER), false)
	@echo "[+] It appears you are not running this command inside the \`Dittobytes Compiler Container\`."
	@echo "[+] You can build it and run in in the root of the Dittobytes project directory."
	@echo "    $ docker buildx build -t dittobytes ."
	@echo "    $ docker run --rm -v ".:/tmp/workdir" -it dittobytes"
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

WIN_AMD64_TARGET            := x86_64-w64-mingw32
WIN_AMD64_DEFINES           := -D__WINDOWS__ -D__AMD64__ -DEntryFunction=shellcode
WIN_AMD64_BEACON_PATH       := $(BUILD_DIR)/$(WIN_AMD64_BEACON_NAME)$(if $(BEACON_NAME),-$(BEACON_NAME))
WIN_AMD64_BEACON_CL1FLAGS   := -target $(WIN_AMD64_TARGET) $(WIN_AMD64_DEFINES) -fuse-ld=lld -O0 -emit-llvm -S -fPIC -ffreestanding -nostdlib -nodefaultlibs -fno-stack-protector -fpass-plugin=./transpilers/intermediate/build/libIntermediateTranspiler.so -Xclang -disable-O0-optnone -fPIC -fno-rtti -fno-exceptions -fno-delayed-template-parsing -fno-modules -fno-fast-math -fno-builtin -fno-elide-constructors -fno-access-control -fno-jump-tables -fno-omit-frame-pointer -fno-ident -fno-inline -fno-inline-functions -mno-red-zone 
WIN_AMD64_BEACON_LLCFLAGS   := -mtriple $(WIN_AMD64_TARGET) -march=x86-64 -O0 --relocation-model=pic $(if $(filter true,$(MM_RANDOM_REGISTER_ALLOCATION)),--fast-randomize-register-allocation)
WIN_AMD64_BEACON_CL2FLAGS   := -target $(WIN_AMD64_TARGET) $(WIN_AMD64_DEFINES) -fuse-ld=lld -e shellcode -fPIC -ffreestanding -nostdlib -nodefaultlibs -fno-stack-protector -mno-red-zone 

$(WIN_AMD64_BEACON_PATH).ll: $(SOURCE_PATH) | $(BUILD_DIR)
	@echo "[+] Compiling $(WIN_AMD64_BEACON_NAME)$(if $(BEACON_NAME),-$(BEACON_NAME))."
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) clang $(WIN_AMD64_BEACON_CL1FLAGS) $< -o $@

$(WIN_AMD64_BEACON_PATH).meta0.mir: $(WIN_AMD64_BEACON_PATH).ll
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) llc $(WIN_AMD64_BEACON_LLCFLAGS) -stop-before=regallocfast -o $@ $<

$(WIN_AMD64_BEACON_PATH).meta1.mir: $(WIN_AMD64_BEACON_PATH).meta0.mir
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) MACHINE_TRANSPILER_STEP=first llc $(WIN_AMD64_BEACON_LLCFLAGS) -load ./transpilers/machine/build/libMachineTranspiler.so --run-pass=MachineTranspiler -o $@ $<

$(WIN_AMD64_BEACON_PATH).meta2.mir: $(WIN_AMD64_BEACON_PATH).meta1.mir
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) llc $(WIN_AMD64_BEACON_LLCFLAGS) -start-before=regallocfast -stop-after=virtregrewriter -o $@ $<

$(WIN_AMD64_BEACON_PATH).meta3.mir: $(WIN_AMD64_BEACON_PATH).meta2.mir
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) MACHINE_TRANSPILER_STEP=last llc $(WIN_AMD64_BEACON_LLCFLAGS) -load ./transpilers/machine/build/libMachineTranspiler.so --run-pass=MachineTranspiler -o $@ $<

$(WIN_AMD64_BEACON_PATH).obj: $(WIN_AMD64_BEACON_PATH).meta3.mir
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) llc $(WIN_AMD64_BEACON_LLCFLAGS) -filetype=obj -start-after=virtregrewriter -o $@ $<

$(WIN_AMD64_BEACON_PATH).lkd: $(WIN_AMD64_BEACON_PATH).obj
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) clang $(WIN_AMD64_BEACON_CL2FLAGS) -o $@ $<

$(WIN_AMD64_BEACON_PATH).bin: $(WIN_AMD64_BEACON_PATH).lkd
	@echo "    - Intermediate compile of $@."
	@$(PYTHON_PATH) ./scripts/make/extract-text-segment.py $< $@

$(WIN_AMD64_BEACON_NAME): $(WIN_AMD64_BEACON_PATH).bin
ifeq ($(DEBUG), false)
	@echo "    - Intermediate cleanup of build files."
	@rm $(WIN_AMD64_BEACON_PATH)*.lkd $(WIN_AMD64_BEACON_PATH)*.obj $(WIN_AMD64_BEACON_PATH)*.*mir $(WIN_AMD64_BEACON_PATH)*.ll
endif
	@echo "    - Done building $@."

##########################################
## Windows ARM64                        ##
##########################################

WIN_ARM64_TARGET            := aarch64-w64-mingw32
WIN_ARM64_DEFINES           := -D__WINDOWS__ -D__ARM64__ -DEntryFunction=shellcode
WIN_ARM64_BEACON_PATH       := $(BUILD_DIR)/$(WIN_ARM64_BEACON_NAME)$(if $(BEACON_NAME),-$(BEACON_NAME))
WIN_ARM64_BEACON_CL1FLAGS   := -target $(WIN_ARM64_TARGET) $(WIN_ARM64_DEFINES) -fuse-ld=lld -O0 -emit-llvm -S -fPIC -ffreestanding -nostdlib -nodefaultlibs -fno-stack-protector -fpass-plugin=./transpilers/intermediate/build/libIntermediateTranspiler.so -Xclang -disable-O0-optnone -fPIC -fno-rtti -fno-exceptions -fno-delayed-template-parsing -fno-modules -fno-fast-math -fno-builtin -fno-elide-constructors -fno-access-control -fno-jump-tables -fno-omit-frame-pointer -fno-ident -fno-inline -fno-inline-functions -mno-red-zone
WIN_ARM64_BEACON_LLCFLAGS   := -mtriple $(WIN_ARM64_TARGET) -march=aarch64 -O0 --relocation-model=pic $(if $(filter true,$(MM_RANDOM_REGISTER_ALLOCATION)),--fast-randomize-register-allocation)
WIN_ARM64_BEACON_CL2FLAGS   := -target $(WIN_ARM64_TARGET) $(WIN_ARM64_DEFINES) -fuse-ld=lld -e shellcode -fPIC -ffreestanding -nostdlib -nodefaultlibs -fno-stack-protector -mno-red-zone

$(WIN_ARM64_BEACON_PATH).ll: $(SOURCE_PATH) | $(BUILD_DIR)
	@echo "[+] Compiling $(WIN_ARM64_BEACON_NAME)$(if $(BEACON_NAME),-$(BEACON_NAME))."
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) clang $(WIN_ARM64_BEACON_CL1FLAGS) $< -o $@

$(WIN_ARM64_BEACON_PATH).meta0.mir: $(WIN_ARM64_BEACON_PATH).ll
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) llc $(WIN_ARM64_BEACON_LLCFLAGS) -stop-before=regallocfast -o $@ $<

$(WIN_ARM64_BEACON_PATH).meta1.mir: $(WIN_ARM64_BEACON_PATH).meta0.mir
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) MACHINE_TRANSPILER_STEP=first llc $(WIN_ARM64_BEACON_LLCFLAGS) -load ./transpilers/machine/build/libMachineTranspiler.so --run-pass=MachineTranspiler -o $@ $<

$(WIN_ARM64_BEACON_PATH).meta2.mir: $(WIN_ARM64_BEACON_PATH).meta1.mir
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) llc $(WIN_ARM64_BEACON_LLCFLAGS) -start-before=regallocfast -stop-after=virtregrewriter -o $@ $<

$(WIN_ARM64_BEACON_PATH).meta3.mir: $(WIN_ARM64_BEACON_PATH).meta2.mir
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) MACHINE_TRANSPILER_STEP=last llc $(WIN_ARM64_BEACON_LLCFLAGS) -load ./transpilers/machine/build/libMachineTranspiler.so --run-pass=MachineTranspiler -o $@ $<

$(WIN_ARM64_BEACON_PATH).obj: $(WIN_ARM64_BEACON_PATH).meta3.mir
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) llc $(WIN_ARM64_BEACON_LLCFLAGS) -filetype=obj -start-after=virtregrewriter -o $@ $<

$(WIN_ARM64_BEACON_PATH).lkd: $(WIN_ARM64_BEACON_PATH).obj
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) clang $(WIN_ARM64_BEACON_CL2FLAGS) -o $@ $<

$(WIN_ARM64_BEACON_PATH).bin: $(WIN_ARM64_BEACON_PATH).lkd
	@echo "    - Intermediate compile of $@."
	@$(PYTHON_PATH) ./scripts/make/extract-text-segment.py $< $@

$(WIN_ARM64_BEACON_NAME): $(WIN_ARM64_BEACON_PATH).bin
ifeq ($(DEBUG), false)
	@echo "    - Intermediate cleanup of build files."
	@rm $(WIN_ARM64_BEACON_PATH)*.lkd $(WIN_ARM64_BEACON_PATH)*.obj $(WIN_ARM64_BEACON_PATH)*.*mir $(WIN_ARM64_BEACON_PATH)*.ll
endif
	@echo "    - Done building $@."

##########################################
## Linux AMD64                          ##
##########################################

LIN_AMD64_TARGET            := x86_64-linux-gnu
LIN_AMD64_DEFINES           := -D__LINUX__ -D__AMD64__ -DEntryFunction=shellcode
LIN_AMD64_BEACON_PATH       := $(BUILD_DIR)/$(LIN_AMD64_BEACON_NAME)$(if $(BEACON_NAME),-$(BEACON_NAME))
LIN_AMD64_BEACON_CL1FLAGS   := -target $(LIN_AMD64_TARGET) $(LIN_AMD64_DEFINES) -O0 -emit-llvm -S -fPIC -ffreestanding -nostdlib -nodefaultlibs -fno-stack-protector -fpass-plugin=./transpilers/intermediate/build/libIntermediateTranspiler.so -Xclang -disable-O0-optnone -fPIC -fno-rtti -fno-exceptions -fno-delayed-template-parsing -fno-modules -fno-fast-math -fno-builtin -fno-elide-constructors -fno-access-control -fno-jump-tables -fno-omit-frame-pointer -fno-ident -fno-inline -fno-inline-functions -mno-red-zone
LIN_AMD64_BEACON_LLCFLAGS   := -mtriple $(LIN_AMD64_TARGET) -march=x86-64 -O0 --relocation-model=pic $(if $(filter true,$(MM_RANDOM_REGISTER_ALLOCATION)),--fast-randomize-register-allocation)
LIN_AMD64_BEACON_CL2FLAGS   := -target $(LIN_AMD64_TARGET) $(LIN_AMD64_DEFINES) -fuse-ld=lld -e shellcode -fPIC -ffreestanding -nostdlib -nodefaultlibs -fno-stack-protector -mno-red-zone

$(LIN_AMD64_BEACON_PATH).ll: $(SOURCE_PATH) | $(BUILD_DIR)
	@echo "[+] Compiling $(LIN_AMD64_BEACON_NAME)$(if $(BEACON_NAME),-$(BEACON_NAME))."
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_LIN):$(PATH) clang $(LIN_AMD64_BEACON_CL1FLAGS) $< -o $@

$(LIN_AMD64_BEACON_PATH).meta0.mir: $(LIN_AMD64_BEACON_PATH).ll
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) llc $(LIN_AMD64_BEACON_LLCFLAGS) -stop-before=regallocfast -o $@ $<

$(LIN_AMD64_BEACON_PATH).meta1.mir: $(LIN_AMD64_BEACON_PATH).meta0.mir
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) MACHINE_TRANSPILER_STEP=first llc $(LIN_AMD64_BEACON_LLCFLAGS) -load ./transpilers/machine/build/libMachineTranspiler.so --run-pass=MachineTranspiler -o $@ $<

$(LIN_AMD64_BEACON_PATH).meta2.mir: $(LIN_AMD64_BEACON_PATH).meta1.mir
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) llc $(LIN_AMD64_BEACON_LLCFLAGS) -start-before=regallocfast -stop-after=virtregrewriter -o $@ $<

$(LIN_AMD64_BEACON_PATH).meta3.mir: $(LIN_AMD64_BEACON_PATH).meta2.mir
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) MACHINE_TRANSPILER_STEP=last llc $(LIN_AMD64_BEACON_LLCFLAGS) -load ./transpilers/machine/build/libMachineTranspiler.so --run-pass=MachineTranspiler -o $@ $<

$(LIN_AMD64_BEACON_PATH).obj: $(LIN_AMD64_BEACON_PATH).meta3.mir
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) llc $(LIN_AMD64_BEACON_LLCFLAGS) -filetype=obj -start-after=virtregrewriter -o $@ $<

$(LIN_AMD64_BEACON_PATH).lkd: $(LIN_AMD64_BEACON_PATH).obj
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_LIN):$(PATH) clang $(LIN_AMD64_BEACON_CL2FLAGS) -o $@ $<

$(LIN_AMD64_BEACON_PATH).bin: $(LIN_AMD64_BEACON_PATH).lkd
	@echo "    - Intermediate compile of $@."
	@$(PYTHON_PATH) ./scripts/make/extract-text-segment.py $< $@

$(LIN_AMD64_BEACON_NAME): $(LIN_AMD64_BEACON_PATH).bin
ifeq ($(DEBUG), false)
	@echo "    - Intermediate cleanup of build files."
	@rm $(LIN_AMD64_BEACON_PATH)*.lkd $(LIN_AMD64_BEACON_PATH)*.obj $(LIN_AMD64_BEACON_PATH)*.*mir $(LIN_AMD64_BEACON_PATH)*.ll
endif
	@echo "    - Done building $@."

##########################################
## Linux ARM64                          ##
##########################################

LIN_ARM64_TARGET            := aarch64-linux-gnu
LIN_ARM64_DEFINES           := -D__LINUX__ -D__ARM64__ -DEntryFunction=shellcode
LIN_ARM64_BEACON_PATH       := $(BUILD_DIR)/$(LIN_ARM64_BEACON_NAME)$(if $(BEACON_NAME),-$(BEACON_NAME))
LIN_ARM64_BEACON_CL1FLAGS   := -target $(LIN_ARM64_TARGET) $(LIN_ARM64_DEFINES) -O0 -emit-llvm -S -fPIC -ffreestanding -nostdlib -nodefaultlibs -fno-stack-protector -fpass-plugin=./transpilers/intermediate/build/libIntermediateTranspiler.so -Xclang -disable-O0-optnone -fPIC -fno-rtti -fno-exceptions -fno-delayed-template-parsing -fno-modules -fno-fast-math -fno-builtin -fno-elide-constructors -fno-access-control -fno-jump-tables -fno-omit-frame-pointer -fno-ident -fno-inline -fno-inline-functions -mno-red-zone
LIN_ARM64_BEACON_LLCFLAGS   := -mtriple $(LIN_ARM64_TARGET) -march=aarch64 -O0 --relocation-model=pic $(if $(filter true,$(MM_RANDOM_REGISTER_ALLOCATION)),--fast-randomize-register-allocation)
LIN_ARM64_BEACON_CL2FLAGS   := -target $(LIN_ARM64_TARGET) $(LIN_ARM64_DEFINES) -fuse-ld=lld -e shellcode -fPIC -ffreestanding -nostdlib -nodefaultlibs -fno-stack-protector -mno-red-zone

$(LIN_ARM64_BEACON_PATH).ll: $(SOURCE_PATH) | $(BUILD_DIR)
	@echo "[+] Compiling $(LIN_ARM64_BEACON_NAME)$(if $(BEACON_NAME),-$(BEACON_NAME))."
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_LIN):$(PATH) clang $(LIN_ARM64_BEACON_CL1FLAGS) $< -o $@

$(LIN_ARM64_BEACON_PATH).meta0.mir: $(LIN_ARM64_BEACON_PATH).ll
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) llc $(LIN_ARM64_BEACON_LLCFLAGS) -stop-before=regallocfast -o $@ $<

$(LIN_ARM64_BEACON_PATH).meta1.mir: $(LIN_ARM64_BEACON_PATH).meta0.mir
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) MACHINE_TRANSPILER_STEP=first llc $(LIN_ARM64_BEACON_LLCFLAGS) -load ./transpilers/machine/build/libMachineTranspiler.so --run-pass=MachineTranspiler -o $@ $<

$(LIN_ARM64_BEACON_PATH).meta2.mir: $(LIN_ARM64_BEACON_PATH).meta1.mir
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) llc $(LIN_ARM64_BEACON_LLCFLAGS) -start-before=regallocfast -stop-after=virtregrewriter -o $@ $<

$(LIN_ARM64_BEACON_PATH).meta3.mir: $(LIN_ARM64_BEACON_PATH).meta2.mir
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) MACHINE_TRANSPILER_STEP=last llc $(LIN_ARM64_BEACON_LLCFLAGS) -load ./transpilers/machine/build/libMachineTranspiler.so --run-pass=MachineTranspiler -o $@ $<

$(LIN_ARM64_BEACON_PATH).obj: $(LIN_ARM64_BEACON_PATH).meta3.mir
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_WIN):$(PATH) llc $(LIN_ARM64_BEACON_LLCFLAGS) -filetype=obj -start-after=virtregrewriter -o $@ $<

$(LIN_ARM64_BEACON_PATH).lkd: $(LIN_ARM64_BEACON_PATH).obj
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_LIN):$(PATH) clang $(LIN_ARM64_BEACON_CL2FLAGS) -o $@ $<

$(LIN_ARM64_BEACON_PATH).bin: $(LIN_ARM64_BEACON_PATH).lkd
	@echo "    - Intermediate compile of $@."
	@$(PYTHON_PATH) ./scripts/make/extract-text-segment.py $< $@

$(LIN_ARM64_BEACON_NAME): $(LIN_ARM64_BEACON_PATH).bin
ifeq ($(DEBUG), false)
	@echo "    - Intermediate cleanup of build files."
	@rm $(LIN_ARM64_BEACON_PATH)*.lkd $(LIN_ARM64_BEACON_PATH)*.obj $(LIN_ARM64_BEACON_PATH)*.*mir $(LIN_ARM64_BEACON_PATH)*.ll
endif
	@echo "    - Done building $@."

##########################################
## MacOS AMD64                          ##
##########################################

MAC_AMD64_TARGET            := x86_64-apple-darwin
MAC_AMD64_DEFINES           := -D__MACOS__ -D__AMD64__ -DEntryFunction=main
MAC_AMD64_BEACON_PATH       := $(BUILD_DIR)/$(MAC_AMD64_BEACON_NAME)$(if $(BEACON_NAME),-$(BEACON_NAME))
MAC_AMD64_BEACON_CL1FLAGS   := -target $(MAC_AMD64_TARGET) $(MAC_AMD64_DEFINES) -O0 -emit-llvm -S -fPIC -ffreestanding -nostdlib -nodefaultlibs -fno-stack-protector -isysroot/opt/macos-sdk/MacOSX15.4.sdk/ -I/opt/macos-sdk/MacOSX15.4.sdk/usr/include -fpass-plugin=./transpilers/intermediate/build/libIntermediateTranspiler.so -Xclang -disable-O0-optnone -fPIC -fno-rtti -fno-exceptions -fno-delayed-template-parsing -fno-modules -fno-fast-math -fno-builtin -fno-elide-constructors -fno-access-control -fno-jump-tables -fno-omit-frame-pointer -fno-ident -fno-inline -fno-inline-functions -mno-red-zone
MAC_AMD64_BEACON_LLCFLAGS   := -mtriple $(MAC_AMD64_TARGET) -march=x86-64 -O0 --relocation-model=pic $(if $(filter true,$(MM_RANDOM_REGISTER_ALLOCATION)),--fast-randomize-register-allocation)
MAC_AMD64_BEACON_CL2FLAGS   := -target $(MAC_AMD64_TARGET) $(MAC_AMD64_DEFINES) -fuse-ld=lld -fPIC -ffreestanding -nostdlib -nodefaultlibs -fno-stack-protector -mno-red-zone -isysroot/opt/macos-sdk/MacOSX15.4.sdk/ -L/opt/macos-sdk/MacOSX15.4.sdk/usr/lib

$(MAC_AMD64_BEACON_PATH).ll: $(SOURCE_PATH) | $(BUILD_DIR)
	@echo "[+] Compiling $(MAC_AMD64_BEACON_NAME)$(if $(BEACON_NAME),-$(BEACON_NAME))."
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_MAC):$(PATH) clang $(MAC_AMD64_BEACON_CL1FLAGS) $< -o $@

$(MAC_AMD64_BEACON_PATH).meta0.mir: $(MAC_AMD64_BEACON_PATH).ll
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_MAC):$(PATH) llc $(MAC_AMD64_BEACON_LLCFLAGS) -stop-before=regallocfast -o $@ $<

$(MAC_AMD64_BEACON_PATH).meta1.mir: $(MAC_AMD64_BEACON_PATH).meta0.mir
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_MAC):$(PATH) MACHINE_TRANSPILER_STEP=first llc $(MAC_AMD64_BEACON_LLCFLAGS) -load ./transpilers/machine/build/libMachineTranspiler.so --run-pass=MachineTranspiler -o $@ $<

$(MAC_AMD64_BEACON_PATH).meta2.mir: $(MAC_AMD64_BEACON_PATH).meta1.mir
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_MAC):$(PATH) llc $(MAC_AMD64_BEACON_LLCFLAGS) -start-before=regallocfast -stop-after=virtregrewriter -o $@ $<

$(MAC_AMD64_BEACON_PATH).meta3.mir: $(MAC_AMD64_BEACON_PATH).meta2.mir
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_MAC):$(PATH) MACHINE_TRANSPILER_STEP=last llc $(MAC_AMD64_BEACON_LLCFLAGS) -load ./transpilers/machine/build/libMachineTranspiler.so --run-pass=MachineTranspiler -o $@ $<

$(MAC_AMD64_BEACON_PATH).obj: $(MAC_AMD64_BEACON_PATH).meta3.mir
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_MAC):$(PATH) llc $(MAC_AMD64_BEACON_LLCFLAGS) -filetype=obj -start-after=virtregrewriter -o $@ $<

$(MAC_AMD64_BEACON_PATH).lkd: $(MAC_AMD64_BEACON_PATH).obj
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_MAC):$(PATH) clang $(MAC_AMD64_BEACON_CL2FLAGS) -o $@ $<

$(MAC_AMD64_BEACON_PATH).bin: $(MAC_AMD64_BEACON_PATH).lkd
	@echo "    - Intermediate compile of $@."
	@$(PYTHON_PATH) ./scripts/make/extract-text-segment.py $< $@

$(MAC_AMD64_BEACON_NAME): $(MAC_AMD64_BEACON_PATH).bin
ifeq ($(DEBUG), false)
	@echo "    - Intermediate cleanup of build files."
	@rm $(MAC_AMD64_BEACON_PATH)*.lkd $(MAC_AMD64_BEACON_PATH)*.obj $(MAC_AMD64_BEACON_PATH)*.*mir $(MAC_AMD64_BEACON_PATH)*.ll
endif
	@echo "    - Done building $@."

##########################################
## MacOS ARM64                          ##
##########################################

MAC_ARM64_TARGET         := arm64-apple-darwin
MAC_ARM64_DEFINES        := -D__MACOS__ -D__ARM64__ -DEntryFunction=main
MAC_ARM64_BEACON_PATH       := $(BUILD_DIR)/$(MAC_ARM64_BEACON_NAME)$(if $(BEACON_NAME),-$(BEACON_NAME))
MAC_ARM64_BEACON_CL1FLAGS   := -target $(MAC_ARM64_TARGET) $(MAC_ARM64_DEFINES) -O0 -emit-llvm -S -fPIC -ffreestanding -nostdlib -nodefaultlibs -fno-stack-protector -isysroot/opt/macos-sdk/MacOSX15.4.sdk/ -I/opt/macos-sdk/MacOSX15.4.sdk/usr/include -fpass-plugin=./transpilers/intermediate/build/libIntermediateTranspiler.so -Xclang -disable-O0-optnone -fPIC -fno-rtti -fno-exceptions -fno-delayed-template-parsing -fno-modules -fno-fast-math -fno-builtin -fno-elide-constructors -fno-access-control -fno-jump-tables -fno-omit-frame-pointer -fno-ident -fno-inline -fno-inline-functions -mno-red-zone
MAC_ARM64_BEACON_LLCFLAGS   := -mtriple $(MAC_ARM64_TARGET) -march=aarch64 -O0 --relocation-model=pic $(if $(filter true,$(MM_RANDOM_REGISTER_ALLOCATION)),--fast-randomize-register-allocation)
MAC_ARM64_BEACON_CL2FLAGS   := -target $(MAC_ARM64_TARGET) $(MAC_ARM64_DEFINES) -fuse-ld=lld -fPIC -ffreestanding -nostdlib -nodefaultlibs -fno-stack-protector -mno-red-zone -isysroot/opt/macos-sdk/MacOSX15.4.sdk/ -L/opt/macos-sdk/MacOSX15.4.sdk/usr/lib

$(MAC_ARM64_BEACON_PATH).ll: $(SOURCE_PATH) | $(BUILD_DIR)
	@echo "[+] Compiling $(MAC_ARM64_BEACON_NAME)$(if $(BEACON_NAME),-$(BEACON_NAME))."
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_MAC):$(PATH) clang $(MAC_ARM64_BEACON_CL1FLAGS) $< -o $@

$(MAC_ARM64_BEACON_PATH).meta0.mir: $(MAC_ARM64_BEACON_PATH).ll
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_MAC):$(PATH) llc $(MAC_ARM64_BEACON_LLCFLAGS) -stop-before=regallocfast -o $@ $<

$(MAC_ARM64_BEACON_PATH).meta1.mir: $(MAC_ARM64_BEACON_PATH).meta0.mir
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_MAC):$(PATH) MACHINE_TRANSPILER_STEP=first llc $(MAC_ARM64_BEACON_LLCFLAGS) -load ./transpilers/machine/build/libMachineTranspiler.so --run-pass=MachineTranspiler -o $@ $<

$(MAC_ARM64_BEACON_PATH).meta2.mir: $(MAC_ARM64_BEACON_PATH).meta1.mir
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_MAC):$(PATH) llc $(MAC_ARM64_BEACON_LLCFLAGS) -start-before=regallocfast -stop-after=virtregrewriter -o $@ $<

$(MAC_ARM64_BEACON_PATH).meta3.mir: $(MAC_ARM64_BEACON_PATH).meta2.mir
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_MAC):$(PATH) MACHINE_TRANSPILER_STEP=last llc $(MAC_ARM64_BEACON_LLCFLAGS) -load ./transpilers/machine/build/libMachineTranspiler.so --run-pass=MachineTranspiler -o $@ $<

$(MAC_ARM64_BEACON_PATH).obj: $(MAC_ARM64_BEACON_PATH).meta3.mir
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_MAC):$(PATH) llc $(MAC_ARM64_BEACON_LLCFLAGS) -filetype=obj -start-after=virtregrewriter -o $@ $<

$(MAC_ARM64_BEACON_PATH).lkd: $(MAC_ARM64_BEACON_PATH).obj
	@echo "    - Intermediate compile of $@."
	@PATH=$(LLVM_DIR_MAC):$(PATH) clang $(MAC_ARM64_BEACON_CL2FLAGS) -o $@ $<

$(MAC_ARM64_BEACON_PATH).bin: $(MAC_ARM64_BEACON_PATH).lkd
	@echo "    - Intermediate compile of $@."
	@$(PYTHON_PATH) ./scripts/make/extract-text-segment.py $< $@

$(MAC_ARM64_BEACON_NAME): $(MAC_ARM64_BEACON_PATH).bin
ifeq ($(DEBUG), false)
	@echo "    - Intermediate cleanup of build files."
	@rm $(MAC_ARM64_BEACON_PATH)*.lkd $(MAC_ARM64_BEACON_PATH)*.obj $(MAC_ARM64_BEACON_PATH)*.*mir $(MAC_ARM64_BEACON_PATH)*.ll
endif
	@echo "    - Done building $@."

##########################################
## Utility targets                      ##
##########################################

$(BUILD_DIR):
	@echo "[+] Creating build directory."
	@mkdir -p $(BUILD_DIR)

dependencies:
	@echo "[+] Installing Python dependencies."
	@$(PYTHON_PATH) -m pip install --upgrade pip
	@$(PYTHON_PATH) -m pip install -r scripts/requirements.txt --break-system-packages

clean:
	@echo "[+] Removing beacons from build folder."
	@rm -f $(BUILD_DIR)/beacon-*

clean-beacons: clean

clean-loaders:
	@echo "[+] Calling \`clean\` in loaders makefile."
	@$(MAKE) IS_COMPILER_CONTAINER=$(IS_COMPILER_CONTAINER) --no-print-directory -C ./loaders/ clean

clean-transpilers:
	@echo "[+] Calling \`clean\` in intermediate transpiler makefile."
	@$(MAKE) IS_COMPILER_CONTAINER=$(IS_COMPILER_CONTAINER) --no-print-directory -C ./transpilers/intermediate/ clean
	@echo "[+] Calling \`clean\` in machine transpiler makefile."
	@$(MAKE) IS_COMPILER_CONTAINER=$(IS_COMPILER_CONTAINER) --no-print-directory -C ./transpilers/machine/ clean

clean-extensive: clean-transpilers clean-loaders clean-beacons

list:
	@echo "[+] Available generic targets:"
	@echo "    - beacons                      // (Re-)compile all beacons"
	@echo "    - loaders                      // (Re-)compile all loaders"
	@echo "    - transpilers                  // (Re-)compile the transpilers"
	@echo "    - extensive                    // (Re-)compile everything"
	@echo "[+] Available test targets:"
	@echo "    - test-suite-build             // (Re-)compile all feature tests"
	@echo "    - test-suite-test              // Run all feature tests (for the current architecture)"
	@echo "[+] Available beacon targets:"
	@echo "    - $(WIN_AMD64_BEACON_NAME)             // (Re)compile the Windows beacon for AMD64"
	@echo "    - $(WIN_ARM64_BEACON_NAME)             // (Re)compile the Windows beacon for ARM64"
	@echo "    - $(LIN_AMD64_BEACON_NAME)             // (Re)compile the Linux beacon for AMD64"
	@echo "    - $(LIN_ARM64_BEACON_NAME)             // (Re)compile the Linux beacon for ARM64"
	@echo "    - $(MAC_AMD64_BEACON_NAME)             // (Re)compile the MacOS beacon for AMD64"
	@echo "    - $(MAC_ARM64_BEACON_NAME)             // (Re)compile the MacOS beacon for ARM64"

.PHONY: all dependencies clean \
	$(WIN_AMD64_BEACON_NAME) $(WIN_AMD64_BEACON_PATH) $(WIN_AMD64_BEACON_PATH).exe $(WIN_AMD64_BEACON_PATH).obj $(WIN_AMD64_BEACON_PATH).bin $(WIN_AMD64_BEACON_PATH).lkd $(WIN_AMD64_BEACON_PATH).ll $(WIN_AMD64_BEACON_PATH).meta.mir $(WIN_AMD64_BEACON_PATH).mir \
	$(WIN_ARM64_BEACON_NAME) $(WIN_ARM64_BEACON_PATH) $(WIN_ARM64_BEACON_PATH).exe $(WIN_ARM64_BEACON_PATH).obj $(WIN_ARM64_BEACON_PATH).bin $(WIN_ARM64_BEACON_PATH).lkd $(WIN_ARM64_BEACON_PATH).ll $(WIN_ARM64_BEACON_PATH).meta.mir $(WIN_ARM64_BEACON_PATH).mir \
	$(LIN_AMD64_BEACON_NAME) $(LIN_AMD64_BEACON_PATH) $(LIN_AMD64_BEACON_PATH).exe $(LIN_AMD64_BEACON_PATH).obj $(LIN_AMD64_BEACON_PATH).bin $(LIN_AMD64_BEACON_PATH).lkd $(LIN_AMD64_BEACON_PATH).ll $(LIN_AMD64_BEACON_PATH).meta.mir $(LIN_AMD64_BEACON_PATH).mir \
	$(LIN_ARM64_BEACON_NAME) $(LIN_ARM64_BEACON_PATH) $(LIN_ARM64_BEACON_PATH).exe $(LIN_ARM64_BEACON_PATH).obj $(LIN_ARM64_BEACON_PATH).bin $(LIN_ARM64_BEACON_PATH).lkd $(LIN_ARM64_BEACON_PATH).ll $(LIN_ARM64_BEACON_PATH).meta.mir $(LIN_ARM64_BEACON_PATH).mir \
	$(MAC_AMD64_BEACON_NAME) $(MAC_AMD64_BEACON_PATH) $(MAC_AMD64_BEACON_PATH).exe $(MAC_AMD64_BEACON_PATH).obj $(MAC_AMD64_BEACON_PATH).bin $(MAC_AMD64_BEACON_PATH).lkd $(MAC_AMD64_BEACON_PATH).ll $(MAC_AMD64_BEACON_PATH).meta.mir $(MAC_AMD64_BEACON_PATH).mir \
	$(MAC_ARM64_BEACON_NAME) $(MAC_ARM64_BEACON_PATH) $(MAC_ARM64_BEACON_PATH).exe $(MAC_ARM64_BEACON_PATH).obj $(MAC_ARM64_BEACON_PATH).bin $(MAC_ARM64_BEACON_PATH).lkd $(MAC_ARM64_BEACON_PATH).ll $(MAC_ARM64_BEACON_PATH).meta.mir $(MAC_ARM64_BEACON_PATH).mir