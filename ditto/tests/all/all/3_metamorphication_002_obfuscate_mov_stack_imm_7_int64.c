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
 * We test AMD64 as `mov reg, imm` as `mov [reg+offset], imm` with int64_t does not use a direct store-to-stack instruction.
 * We test ARM64 as `mov reg, imm` as `mov [reg+offset], imm` does not exist in ARM64.

 *            OS     Arch     Metamorphication                Test                              Argument(s)                         Description
 * @verify    all    amd64    transform_reg_mov_immediates    hex_not_present                   48B80000000000000080                Is `mov dword [rbp+var_a], 0x800000000000000` in HEX.
 * @verify    all    arm64    transform_reg_mov_immediates    hex_not_present                   0800f0d2                            Is `mov w8, #0x800000000000000` in HEX.
 * @verify    all    all      transpiled_1                    minimum_levenshtein_distance      transpiled_2,10                     There must be a minimum % change per compile.
 * @verify    all    all      all                             forensically_clean                None                                All compiled versions must have the minimum amount of potential forensic traces.
 * @verify    all    all      all                             returns                           int64_t,-9223372036854775808,raw    Must be the case without metamorphications.
 */
int64_t EntryFunction() {
    /**
     * Note that we use -9223372036854775807LL - 1LL rather than directly typing -9223372036854775808LL 
     * because some compilers treat the latter as an unsigned integer literal (that would be too large) 
     * with a negative sign applied afterward.
     */
    int64_t bStackVar;
    bStackVar = (int64_t) -9223372036854775807LL - 1LL;
    return bStackVar;
}