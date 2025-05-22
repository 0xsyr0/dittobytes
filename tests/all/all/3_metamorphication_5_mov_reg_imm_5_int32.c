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
 *            OS     Arch     Metamorphication        Test                              Argument(s)             Description
 * @verify    all    amd64    modify_mov_immediate    hex_not_present                   B800000080              Is `mov eax, 0x80000000` in HEX.
 * @verify    all    arm64    modify_mov_immediate    hex_not_present                   0000B052                Is `mov w0, #-0x80000000` in HEX.
 * @verify    all    all      all                     returns                           int32_t,-2147483648     Must be the case without metamorphications.
 */
int32_t EntryFunction() {
    return (int32_t) -2147483648L;
}
