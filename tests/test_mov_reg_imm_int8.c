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
 * @verify AMD64 modify_mov_immediate hex_not_present B080 (`mov al, 0x80` in HEX).
 * @verify AMD64 modify_mov_immediate hex_not_present B880FFFFFF (`mov eax, 0xffffff80` in HEX).
 * @verify ARM64 modify_mov_immediate hex_not_present 00108052 (`mov w0, #0x80` in HEX).
 * @verify ARM64 modify_mov_immediate hex_not_present E00F8012 (`mov w0, #-0x80` in HEX).
 * 
 * @return int8_t The return value to verify: `-128` (which must still be the case after transpilation).
 */
int8_t EntryFunction() {
    return (int8_t) -128;
}
