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
 * The main function of the code to test.
 * 
 * @verify AMD64 hex_not_present 48C7C0FFFFFFFF (`mov rax, 0xffffffffffffffff` in HEX).
 * @verify ARM64 hex_not_present 00008092 (`mov x0, #-1` in HEX).
 * 
 * @return uint64_t The return value to verify: `18446744073709551615` (which must still be the case after transpilation).
 */
uint64_t EntryFunction() {
    return (uint64_t) 18446744073709551615ULL;
}
