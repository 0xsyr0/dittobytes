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
 * @verify hex_not_present FFFFFFFFFFFFFFFF (18446744073709551615 is 0xFFFFFFFFFFFFFFFF in HEX).
 * @verify hex_not_present FFFF (18446744073709551615 is 0xFFFFFFFFFFFFFFFF in HEX, potentially shortened in assembly).
 * 
 * @return uint64_t The return value to verify: `18446744073709551615` (which must still be the case after transpilation).
 */
uint64_t EntryFunction() {
    return (uint64_t) 18446744073709551615;
}
