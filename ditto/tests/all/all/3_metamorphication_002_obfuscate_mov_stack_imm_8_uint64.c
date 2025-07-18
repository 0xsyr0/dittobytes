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
 * We test ARM64 as `mov reg, imm` as `mov [reg+offset], imm` does not exist in ARM64.

 *            OS     Arch     Metamorphication                Test                              Argument(s)                          Description
 * @verify    win    amd64    transform_stack_mov_immediates  hex_not_present                   48C74500FFFFFFFF                     Is `mov dword [rbp+var_a], 0xffffffffffffffff` in HEX.
 * @verify    lin    amd64    transform_stack_mov_immediates  hex_not_present                   48C745F8FFFFFFFF                     Is `mov dword [rbp+var_a], 0xffffffffffffffff` in HEX.
 * @verify    mac    amd64    transform_stack_mov_immediates  hex_not_present                   48C745F8FFFFFFFF                     Is `mov dword [rbp+var_a], 0xffffffffffffffff` in HEX.
 * @verify    all    arm64    transform_reg_mov_immediates    hex_not_present                   08008092                             Is `mov w8, #0xffffffffffffffff` in HEX.
 * @verify    all    all      transpiled_1                    minimum_levenshtein_distance      transpiled_2,10                      There must be a minimum % change per compile.
 * @verify    all    all      all                             forensically_clean                None                                 All compiled versions must have the minimum amount of potential forensic traces.
 * @verify    all    all      all                             returns                           uint64_t,18446744073709551615,raw    Must be the case without metamorphications.
 */
uint64_t EntryFunction() {
    uint64_t bStackVar;
    bStackVar = (uint64_t) 18446744073709551615ULL;
    return bStackVar;
}