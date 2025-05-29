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
 *            OS     Arch     Metamorphication            Test                              Argument(s)        Description
 * @verify    all    amd64    transform_mov_immediates    hex_not_present                   B080               Is `mov w0, #0x80` in HEX.
 * @verify    all    arm64    transform_mov_immediates    hex_not_present                   E00F8012           Is `mov w0, #-0x80` in HEX.
 * @verify    all    all      all                         returns                           int8_t,-128        Must be the case without metamorphications.
 */
int8_t EntryFunction() {
    return (int8_t) -128;
}
