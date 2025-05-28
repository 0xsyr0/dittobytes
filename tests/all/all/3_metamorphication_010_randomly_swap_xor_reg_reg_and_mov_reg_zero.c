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
 * 
 * @verify    all    amd64    modify_xor_reg_reg      hex_not_present                   31C0                             Is `xor, eax, eax` in HEX.
 * @verify    win    arm64    modify_xor_reg_reg      hex_not_present                   E0031F2A                         Is `mov w0, wzr` in HEX.
 * @verify    lin    arm64    modify_xor_reg_reg      hex_not_present                   E0031F2A                         Is `mov w0, wzr` in HEX.
 * @verify    mac    arm64    modify_xor_reg_reg      hex_not_present                   00008052                         Is `mov w0, wzr` in HEX.
 * @verify    all    all      all                     returns                           uint32_t,0                       Must be the case without metamorphications.
 */
uint32_t EntryFunction() {
    return 0UL;
}