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
 * @verify hex_not_present FFFF (65535 is 0xFFFF in HEX).
 * 
 * @return uint16_t The return value to verify: `65535` (which must still be the case after transpilation).
 */
uint16_t EntryFunction() {
    return (uint16_t) 65535;
}
