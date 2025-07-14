/**
 * GNU General Public License, version 2.0.
 *
 * Copyright (c) 2025 Tijme Gommers (@tijme).
 *
 * This source code file is part of Dittobytes. Dittobytes is 
 * licensed under GNU General Public License, version 2.0, and 
 * you are free to use, modify, and distribute this file under 
 * its terms. However, any modified versions of this file must 
 * include this same license and copyright notice.
 */

#include <stdint.h>

#include <windows.h>

/**
 * The main function of your shellcode/payload. In this example we solely return a value.
 *
 * Please note that you CANNOT use any API calls unless you resolve them by yourself. 
 * Take at the following examples to learn how to utilize OS API calls.
 *
 * Examples:
 *   - A Cobalt Strike BOF: `./examples/example-bof/example-bof.c`
 *   - Popping a calculator: `./examples/example-calc/example-calc.c`
 *   - Using C++ instead of C: `./examples/example-cpp/example-cpp.cpp`
 * 
 * 
 * @return uint64_t A return value of your shellcode.
 */
uint64_t EntryFunction() {
    return 0x1234567812345678ULL;
}
