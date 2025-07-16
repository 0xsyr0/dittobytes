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

/**
 * Get the ASCII value of 'H'.
 * 
 * @return uint8_t The value of character 'H' (72).
 */
uint8_t get_1_h();

/**
 * Get the 11th character from a string literal.
 * 
 * @return uint8_t The value of character at index 10 in "Hello Universe!".
 */
uint8_t get_2_e();

/**
 * Get the 360th character from a long string literal.
 * 
 * @return uint8_t The value of the character at index 359.
 */
uint8_t get_3_l();

/**
 * Get the 14th character from "Greetings World!".
 * 
 * @return uint8_t The value of character 'l'.
 */
uint8_t get_4_l();

/**
 * Get the character at a dynamic index from "Goodbye Globe!".
 * 
 * @param int index The index to retrieve.
 * @return uint8_t The value at the given index.
 */
uint8_t get_5_o(int index);

/**
 * Get the character at a dynamic index from "aaa\x0\x0.\x0\x0bbb".
 * 
 * @param int index The index to retrieve.
 * @return uint8_t The value at the given index.
 */
uint8_t get_6_dot(int index);

/**
 * The main function of the code to test.
 * 
 * This test focusses on string storage and usage, to verify whether strings are stored correctly (inline).
 *
 *            OS     Arch     Metamorphication        Test                              Argument(s)                      Description
 * @verify    all    all      transpiled_1            minimum_levenshtein_distance      transpiled_2,5                   There must be a minimum % change per compile.
 * @verify    all    all      all                     forensically_clean                None                             All compiled versions must have the minimum amount of potential forensic traces.
 * @verify    all    all      all                     returns                           uint8_t,7                        Must be the case without metamorphications (ASCII values: 72+101+108+108+111).
 */
uint8_t EntryFunction() {
    uint8_t h = get_1_h();
    uint8_t e = get_2_e();
    uint8_t l_1 = get_3_l();
    uint8_t l_2 = get_4_l();
    uint8_t o = get_5_o(10);
    uint8_t dot = get_6_dot(5);

    return (h+e+l_1+l_2+o+dot) % 49; // 546 % 49 = 7
}

/**
 * Get the ASCII value of 'H'.
 * 
 * @return uint8_t The value of character 'H' (72).
 */
uint8_t get_1_h() {
    char* test = "Hello World!";
    return test[0];
}

/**
 * Get the 11th character from a string literal.
 * 
 * @return uint8_t The value of character at index 10 in "Hello Universe!".
 */
uint8_t get_2_e() {
    char test[] = "Hello Universe!";
    int index = 10;
    return test[index];
}

/**
 * Get the 360th character from a long string literal.
 * 
 * @return uint8_t The value of the character at index 359.
 */
uint8_t get_3_l() {
    char* test = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Praesent eu tincidunt nisl, nec iaculis mi. Proin at dignissim risus. Suspendisse et nulla orci. Pellentesque tempor laoreet blandit. Curabitur ac tortor sed mauris sagittis pulvinar. Phasellus libero magna, suscipit ac blandit at, malesuada non tellus. Etiam sodales euismod dolor, ut accumsan magna aliquet et.";

    int a = test[0] != 'L';
    int b = test[368] != '.';   

    return test[359] + a + b;
}

/**
 * Get the 14th character from "Greetings World!".
 * 
 * @return uint8_t The value of character 'l'.
 */
uint8_t get_4_l() {
    char* test = "Greetings World!";
    char l = test[13];
    return l;
}

/**
 * Get the character at a dynamic index from "Goodbye Globe!".
 * 
 * @param int index The index to retrieve.
 * @return uint8_t The value at the given index.
 */
uint8_t get_5_o(int index) {
    char test[] = "Goodbye Globe!";
    return test[index];
}

/**
 * Get the character at a dynamic index from "aaa\x0\x0.\x0\x0bbb".
 * 
 * @param int index The index to retrieve.
 * @return uint8_t The value at the given index.
 */
uint8_t get_6_dot(int index) {
    char test[10];

    test[0] = 'a';
    test[1] = 'a';
    test[2] = 'a';

    test[5] = '.';

    test[7] = 'b';
    test[8] = 'b';
    test[9] = 'b';

    return test[index];
}