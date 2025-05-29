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
 *            OS     Arch     Metamorphication            Test                              Argument(s)                      Description
 * @verify    all    amd64    transform_mov_immediates    hex_not_present                   48B80000000000000080             Is `movabs rax, 0x8000000000000000` in HEX.
 * @verify    all    arm64    transform_mov_immediates    hex_not_present                   0000F0D2                         Is `mov x0, #-0x8000000000000000` in HEX.
 * @verify    all    all      all                         returns                           int64_t,-9223372036854775808     Must be the case without metamorphications.
 */
int64_t EntryFunction() {
    /**
     * Note that we use -9223372036854775807LL - 1LL rather than directly typing -9223372036854775808LL 
     * because some compilers treat the latter as an unsigned integer literal (that would be too large) 
     * with a negative sign applied afterward.
     */
    return (int64_t) -9223372036854775807LL - 1LL;
}
