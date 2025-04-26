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

BUILD_DIR              := build
IS_CONTAINER           := $(shell if [ "$(IS_CONTAINER)" = "true" ] || [ -f /.dittobytes-env-all-encompassing ]; then echo "true"; else echo "false"; fi)

##########################################
## Default run                          ##
##########################################

all: check_environment $(BUILD_DIR)
	@mkdir -p build 
	@cd build && cmake ../src/
	@cd build && cmake --build . -- -s

##########################################
## Environment check                    ##
##########################################

check_environment:
ifeq ($(IS_CONTAINER), false)
	@echo "[+] It appears you are not running this command inside the \`Dittobytes Transpilers Compiler Container\`."
	@echo "[+] You can build it and run in in the root of the Dittobytes project directory."
	@echo "    $ docker buildx build -t dittobytes-transpilers -f ./transpilers/Dockerfile ."
	@echo "    $ docker run --rm -v ".:/tmp/workdir" -it dittobytes-transpilers"
	@read -p "[+] Do you want to continue anyway? (y/N) " CONTINUE && \
	case "$$CONTINUE" in \
		[yY][eE][sS]|[yY]) echo "[+] Continuing outside container..." ;; \
		*) echo "[!] Aborting." && exit 1 ;; \
	esac
	$(eval IS_CONTAINER := true)
endif

##########################################
## Utility targets                      ##
##########################################

$(BUILD_DIR):
	@echo "[+] Creating intermediate transpiler build folder."
	@mkdir -p $(BUILD_DIR)

clean:
	@echo "[+] Removing intermediate transpiler from build folder."
	@rm -rf $(BUILD_DIR)/*

.PHONY: all clean