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

/**
 * The main function of your shellcode. In this example we solely return a value.
 * 
 * Popping `calc.exe`:
 *     If you want a more extensive example of what Dittobytes can compile, instead of solely returning a 
 *     number, the following file contains a full example to pop `calc.exe` on Windows AMD64: 
 *     `./examples/example-calc/example-calc.c`
 * 
 * Using C++ instead of C-code:
 *     If you want to use C++, rename this file so it has the `cpp` extension, then modify the `makefile` to 
 *     point `SOURCE_PATH` to the new filename. Also ensure `extern "C"` is prepended to the `EntryFunction`
 *     function in this file. The following file contains a full example to use C++:
 *     `./examples/example-cpp/example-cpp.cpp`
 * 
 * @return uint64_t A return value of your shellcode.
 */
uint64_t EntryFunction() {
    return 0x1234567812345678ULL;
}
