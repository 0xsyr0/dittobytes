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
#include <stdbool.h>

/**
 * Get `7`.
 *
 * @return uint8_t The result.
 */
uint8_t seven();

/**
 * Return a deterministic uint8_t.
 *
 * @return uint8_t The return value to verify: `7` (which must still be the case after transpilation).
 */
uint8_t EntryFunction() {
	uint8_t result = seven();
    return result;
}

/**
 * Get `7`.
 *
 * @return uint8_t The result.
 */
uint8_t seven() {
    return 7;
}