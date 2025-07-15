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
 * @verify    all    amd64    transform_mov_immediates    hex_not_present                   B0FF               Is `mov al, 0xff` in HEX.
 * @verify    win    arm64    transform_mov_immediates    hex_not_present                   00008012           Is `mov w0, #0xff` in HEX.
 * @verify    lin    arm64    transform_mov_immediates    hex_not_present                   00008012           Is `mov w0, #0xff` in HEX.
 * @verify    mac    arm64    transform_mov_immediates    hex_not_present                   E01F8052           Is `mov w0, #0xff` in HEX.
 * @verify    all    all      transpiled_1                minimum_levenshtein_distance      transpiled_2,10    There must be a minimum % change per compile.
 * @verify    all    all      all                         forensically_clean                None                             All compiled versions must have the minimum amount of potential forensic traces.
 * @verify    all    all      all                         returns                           uint8_t,255        Must be the case without metamorphications.
 */
uint8_t EntryFunction() {
    return (uint8_t) 255;
}
