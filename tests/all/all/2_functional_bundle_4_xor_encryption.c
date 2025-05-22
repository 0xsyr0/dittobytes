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
#include <string.h>

/** 
 * Run bitwise XOR on text using password.
 *
 * @param char* plainv The plaintext value.
 * @param size_t plainc The length of the plaintext value.
 * @param char* passwv The passord value.
 * @param size_t passwc The length of the password value.
 */
void xorcrypt(char* plainv, size_t plainc, char* passwv, size_t passwc);

/**
 * Return a deterministic uint8_t.
 *
 *            OS     Arch     Metamorphication        Test                              Argument(s)                      Description
 * @verify    all    all      all                     returns                           uint8_t,2                        Must be the case without metamorphications.
 */
uint8_t EntryFunction() {
    char* plainv = "this is a plaintext string";
    char* passwv = "Hunter2";

    xorcrypt(plainv, 26, passwv, 7);

    return (uint8_t) plainv[25];
}

/** 
 * Run bitwise XOR on text using password.
 *
 * @param char* plainv The plaintext value.
 * @param size_t plainc The length of the plaintext value.
 * @param char* passwv The passord value.
 * @param size_t passwc The length of the password value.
 */
void xorcrypt(char* plainv, size_t plainc, char* passwv, size_t passwc) {
    size_t i;

    for(i = 0; i < plainc; i ++) {
        plainv[i] = plainv[i] ^ passwv[i % passwc];
    }
}