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

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * Check whether a given number is a prime.
 *
 * @param int num The number to check.
 * @return bool true if prime, false otherwise.
 */
bool is_prime(int num);

/**
 * The main function of the code to test.
 * 
 * This test focusses on ensuring that complex code still functions. It's goal is not so much to check whether
 * certain specific transpilations work correclty.
 * 
 * @verify ANY hex_not_present 199901 (104729 is 0x199901 in HEX for Little Endian).
 * @verify ANY hex_not_present 019919 (104729 is 0x019919 in HEX for Big Endian).
 * 
 * @return uint32_t The return value to verify: `104729` (which must still be the case after transpilation).
 */
uint32_t EntryFunction() {
    int count = 0;
    int num = 2;
    int result = 0;

    while (count < 10000) {
        if (is_prime(num)) {
            ++count;
            if (count == 10000)
                result = num;
        }
        ++num;
    }

    return result;
}

/**
 * Check whether a given number is a prime.
 *
 * @param int num The number to check.
 * @return bool true if prime, false otherwise.
 */
bool is_prime(int num) {
    if (num < 2) return false;
    for (int i = 2; i * i <= num; ++i)
        if (num % i == 0) return false;
    return true;
}