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
 * certain specific transpilations work correctly.
 * 
 *            OS     Arch     Metamorphication        Test                              Argument(s)                      Description
 * @verify    all    all      transpiled_1            minimum_levenshtein_distance      transpiled_2,20                  There must be a minimum % change per compile.
 * @verify    all    all      all                     returns                           uint32_t,104729                  Must be the case without metamorphications.
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