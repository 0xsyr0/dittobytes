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
 * Custom struct definition
 */
typedef struct {
    uint64_t a;
    uint64_t b;
    uint64_t c;
} MyCustomStruct;

/**
 * The main function of the code to test.
 *
 *            OS     Arch     Metamorphication        Test                              Argument(s)                      Description
 * @verify    all    all      all                     returns                           uint64_t,6352075299651498008     Must be the case without metamorphications.
 */
uint64_t EntryFunction() {
    // Usually stored in `.rodata`
    const MyCustomStruct result = {
        0xDEADBEEFCAFEBABE,
        0x58271AD48E42B018,
        0x0123456789ABCDEF
    };

    return (uint64_t) result.b;
}