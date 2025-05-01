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
 * @verify AMD64 hex_not_present B800000080 (`mov eax, 0x80000000` in HEX).
 * @verify ARM64 hex_not_present 0000B052 (`mov w0, #-0x80000000` in HEX).
 * 
 * @return int32_t The return value to verify: `-2147483648` (which must still be the case after transpilation).
 */
int32_t EntryFunction() {
    return (int32_t) -2147483648L;
}
