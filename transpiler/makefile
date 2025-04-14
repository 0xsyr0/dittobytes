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

BUILD_DIR              := build
IS_DOCKER              := $(shell test -f /.dockertranspilerenv && echo "true" || echo "false")

##########################################
## Default run                          ##
##########################################
all: check_environment $(BUILD_DIR)
	mkdir -p build 
	cd build && cmake -DLT_LLVM_INSTALL_DIR=$LLVM_PATH ../src/
	cd build && cmake --build . 

##########################################
## Environment check                    ##
##########################################
check_environment:
ifeq ($(IS_DOCKER), false)
	@echo "[+] It appears you are not running this command inside the Dittobytes transpiler Docker container"
	@echo "[+] You can build it with: docker build -t dittobytes-transpiler ."
	@echo "[+] You can run it with: docker run --rm -v ".:/tmp/workdir" -it dittobytes-transpiler"
	@read -p "[+] Do you want to continue anyway? (y/N) " CONTINUE && \
	case "$$CONTINUE" in \
		[yY][eE][sS]|[yY]) echo "[+] Continuing outside Docker..." ;; \
		*) echo "[!] Aborting." && exit 1 ;; \
	esac
endif

##########################################
## Utility targets                      ##
##########################################

# Windows x86_64 beacon
$(BUILD_DIR):
	@echo "[+] Creating build directory"
	@mkdir -p $(BUILD_DIR)

clean:
	@echo "[+] Cleaning build folder"
	@rm -f $(BUILD_DIR)/*

.PHONY: all clean