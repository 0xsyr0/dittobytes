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
 * Matrix size
 */
#define SIZE 3

/**
 * Multiply two matrices and return the result.
 *
 * @param int32_t a[SIZE][SIZE] The first matrix.
 * @param int32_t b[SIZE][SIZE] The second matrix.
 * @param int32_t result[SIZE][SIZE] The result matrix.
 */
void multiply_matrices(int32_t a[SIZE][SIZE], int32_t b[SIZE][SIZE], int32_t result[SIZE][SIZE]);

/**
 * The main function of the code to test.
 * 
 * This test focusses on ensuring that complex code still functions. It's goal is not so much to check whether
 * certain specific transpilations work correctly.
 * 
 *            OS     Arch     Metamorphication        Test                              Argument(s)                      Description
 * @verify    all    all      transpiled_1            minimum_levenshtein_distance      transpiled_2,20                  There must be a minimum % change per compile.
 * @verify    all    all      all                     forensically_clean                None                             All compiled versions must have the minimum amount of potential forensic traces.
 * @verify    all    all      all                     returns                           int32_t,24                       Must be the case without metamorphications.
 */
int32_t EntryFunction() {
    int32_t a[SIZE][SIZE] = {{1, 2, 3}, {-4, 5, 6}, {7, 8, 9}};
    int32_t b[SIZE][SIZE] = {{9, 8, 7}, {6, 5, -4}, {3, 2, 1}};
    int32_t result[SIZE][SIZE];

    multiply_matrices(a, b, result);

    int32_t checksum = 0;
    for (int32_t i = 0; i < SIZE; ++i) {
        for (int32_t j = 0; j < SIZE; ++j) {
            checksum += result[i][j];
        }
    }

    return checksum - 285; // 309 - 285 = 24
}

/**
 * Multiply two matrices and return the result.
 *
 * @param int32_t a[SIZE][SIZE] The first matrix.
 * @param int32_t b[SIZE][SIZE] The second matrix.
 * @param int32_t result[SIZE][SIZE] The result matrix.
 */
void multiply_matrices(int32_t a[SIZE][SIZE], int32_t b[SIZE][SIZE], int32_t result[SIZE][SIZE]) {
    for (int32_t i = 0; i < SIZE; ++i) {
        for (int32_t j = 0; j < SIZE; ++j) {
            result[i][j] = 0;
            for (int32_t k = 0; k < SIZE; ++k) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}