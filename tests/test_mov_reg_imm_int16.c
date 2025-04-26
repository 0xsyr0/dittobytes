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
 * @verify hex_not_present 0080 (-32768 is 0x8000 in HEX for Little Endian).
 * @verify hex_not_present 8000 (-32768 is 0x8000 in HEX for Big Endian).
 *
 * @return int16_t The return value to verify: `-32768` (which must still be the case after transpilation).
 */
int16_t EntryFunction() {
    return (int16_t) -32768;
}
