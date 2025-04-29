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
 * @param int a[SIZE][SIZE] The first matrix.
 * @param int b[SIZE][SIZE] The second matrix.
 * @param int result[SIZE][SIZE] The result matrix.
 */
void multiply_matrices(int a[SIZE][SIZE], int b[SIZE][SIZE], int result[SIZE][SIZE]);

/**
 * The main function of the code to test.
 * 
 * This test focusses on ensuring that complex code still functions. It's goal is not so much to check whether
 * certain specific transpilations work correclty.
 * 
 * @verify ANY hex_not_present 0D8901 (100621 is 0x0D8901 in HEX for Little Endian).
 * @verify ANY hex_not_present 01890D (100621 is 0x01890D in HEX for Big Endian).
 * 
 * @return uint32_t The return value to verify: `100621` (which must still be the case after transpilation).
 */
uint32_t EntryFunction() {
    int a[SIZE][SIZE] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    int b[SIZE][SIZE] = {{9, 8, 7}, {6, 5, 4}, {3, 2, 1}};
    int result[SIZE][SIZE];

    multiply_matrices(a, b, result);

    int checksum = 0;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            checksum += result[i][j];
        }
    }

    return checksum + 100000;
}

/**
 * Multiply two matrices and return the result.
 *
 * @param int a[SIZE][SIZE] The first matrix.
 * @param int b[SIZE][SIZE] The second matrix.
 * @param int result[SIZE][SIZE] The result matrix.
 */
void multiply_matrices(int a[SIZE][SIZE], int b[SIZE][SIZE], int result[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            result[i][j] = 0;
            for (int k = 0; k < SIZE; ++k) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}