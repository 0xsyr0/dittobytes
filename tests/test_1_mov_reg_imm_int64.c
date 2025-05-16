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
 * @verify AMD64 modify_mov_immediate hex_not_present 48B80000000000000080 (`movabs rax, 0x8000000000000000` in HEX).
 * @verify ARM64 modify_mov_immediate hex_not_present 0000F0D2 (`mov x0, #-0x8000000000000000` in HEX).
 * 
 * @return int64_t The return value to verify: `-9223372036854775808` (which must still be the case after transpilation).
 */
int64_t EntryFunction() {
    /**
     * Note that we use -9223372036854775807LL - 1LL rather than directly typing -9223372036854775808LL 
     * because some compilers treat the latter as an unsigned integer literal (that would be too large) 
     * with a negative sign applied afterward.
     */
    return (int64_t) -9223372036854775807LL - 1LL;
}
