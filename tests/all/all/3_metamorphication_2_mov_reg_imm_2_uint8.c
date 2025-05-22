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
 *            OS     Arch     Metamorphication        Test                              Argument(s)        Description
 * @verify    all    amd64    modify_mov_immediate    hex_not_present                   B0FF               Is `mov al, 0xff` in HEX.
 * @verify    win    arm64    modify_mov_immediate    hex_not_present                   00008012           Is `mov w0, #0xff` in HEX.
 * @verify    lin    arm64    modify_mov_immediate    hex_not_present                   00008012           Is `mov w0, #0xff` in HEX.
 * @verify    mac    arm64    modify_mov_immediate    hex_not_present                   E01F8052           Is `mov w0, #0xff` in HEX.
 * @verify    all    all      all                     returns                           uint8_t,255        Must be the case without metamorphications.
 */
uint8_t EntryFunction() {
    return (uint8_t) 255;
}
