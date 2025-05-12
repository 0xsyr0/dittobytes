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
 * A deterministic function that performs multiple register-heavy computations.
 *
 * This function is designed to test whether value assignments to registers
 * and intermediate computations are randomized after compilation.
 * 
 * We use `0x19A47B22FAB903DA` to check whether the binary is original, because this value is not shortend (in contrast to e.g. `0x1111111111111111`).
 * 
 * @verify AMD64 modify_mov_immediate hex_not_present DA03B9FA227BA419 (`0x19A47B22FAB903DA` in HEX).
 * @verify ARM64 modify_mov_immediate hex_not_present 487B80D2 (`mov x8,#0x3da` in HEX).
 * @verify ANY random_register_allocation not_identical_to original (this compilation must be different than the original compilation).
 * 
 * @return uint64_t The return value to verify: `11394632769328784690` (which must still be the case after transpilation).
 */
uint64_t EntryFunction() {
    uint64_t r0 = 1;

    uint64_t r1 = 0x00000000000000F0;
    uint64_t r2 = 0x1111111111111111;
    uint64_t r3 = r1 + r2;
    
    r3 = r3 + r0;
    r3 = r3 - r0;

    uint64_t r4 = 0x2222222222222222;
    uint64_t r5 = r4 ^ r3;
    
    r5 = r5 - r0;
    r5 = r5 + r0;

    uint64_t r6 = 0x19A47B22FAB903DA;
    uint64_t r7 = r6 & r5;

    uint64_t r8 = r7 << 4;
    uint64_t r9 = r8 | 0xAAAAAAAAAAAAAAAA;

    uint64_t r10 = r9 >> 2;
    uint64_t r11 = ~r10;
    
    r11 = r11 + r0 - r0;
    r11 = r11 - r0 + r0;

    uint64_t r12 = r11 + r1;
    uint64_t r13 = r12 ^ r2;
    uint64_t r14 = r13 - r4;

    return r14;
}
