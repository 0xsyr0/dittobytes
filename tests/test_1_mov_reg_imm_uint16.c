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
 * @verify AMD64 modify_mov_immediate hex_not_present 66B8FFFF (`mov ax, 0xffff` in HEX).
 * @verify AMD64 modify_mov_immediate hex_not_present B8FFFF0000 (`mov eax, 0xffff` in HEX).
 * @verify ARM64 modify_mov_immediate hex_not_present 00008012 (`mov w0, #0xffff` in HEX on non-MacOS).
 * @verify ARM64 modify_mov_immediate hex_not_present E0FF9F52 (`mov w0, #0xffff` in HEX on MacOS).
 * 
 * @return uint16_t The return value to verify: `65535` (which must still be the case after transpilation).
 */
uint16_t EntryFunction() {
    return (uint16_t) 65535;
}
