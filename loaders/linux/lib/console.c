#pragma once

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

/**
 * Standard Input Output.
 * 
 * Defines three variable types, several macros, and various functions for performing input and output.
 * https://www.tutorialspoint.com/c_standard_library/stdio_h.htm
 */
#include <stdio.h>

/**
 * Standard Library.
 * 
 * Defines four variable types, several macros, and various functions for performing general functions.
 * https://www.tutorialspoint.com/c_standard_library/stdlib_h.htm
 */
#include <stdlib.h>

/**
 * Integers.
 * 
 * Defines macros that specify limits of integer types corresponding to types defined in other standard headers.
 * https://pubs.opengroup.org/onlinepubs/009696899/basedefs/stdint.h.html
 */
#include <stdint.h>

/**
 * Time handling.
 * 
 * Defines macros, types, and functions for manipulating date and time.
 * https://pubs.opengroup.org/onlinepubs/007908799/xsh/time.h.html
 */
#include <time.h>

/**
 * Local definitions
 */
#ifndef ENABLE_VERBOSE_PRINT_STATEMENTS
#define ENABLE_VERBOSE_PRINT_STATEMENTS 0x1 // Verbose printing (if positive)
#endif

/**
 * Console color codes
 */
#define COLOR_RESET "\x1b[0m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_RED "\x1b[31m"

/**
 * Define print methods
 */
#define PRINT(...) { \
    time_t tNow; time(&tNow);  struct tm* lpNow = localtime(&tNow); \
    fprintf(stdout, "[INSIGHT %02d:%02d:%02d] ", lpNow->tm_hour, lpNow->tm_min, lpNow->tm_sec); \
    fprintf(stdout, __VA_ARGS__); \
    fprintf(stdout, "\n"); \
    fflush(stdout); \
}

#define PRINT_SUCCESS(...) { \
    time_t tNow; time(&tNow);  struct tm* lpNow = localtime(&tNow); \
    fprintf(stdout, COLOR_GREEN); \
    fprintf(stdout, "[SUCCESS %02d:%02d:%02d] ", lpNow->tm_hour, lpNow->tm_min, lpNow->tm_sec); \
    fprintf(stdout, __VA_ARGS__); \
    fprintf(stdout, "\n"); \
    fprintf(stdout, COLOR_RESET); \
    fflush(stdout); \
}

#define PRINT_WARNING(...) { \
    time_t tNow; time(&tNow);  struct tm* lpNow = localtime(&tNow); \
    fprintf(stdout, COLOR_YELLOW); \
    fprintf(stdout, "[WARNING %02d:%02d:%02d] ", lpNow->tm_hour, lpNow->tm_min, lpNow->tm_sec); \
    fprintf(stdout, __VA_ARGS__); \
    fprintf(stdout, "\n"); \
    fprintf(stdout, COLOR_RESET); \
    fflush(stdout); \
}

#define PRINT_FAILURE(...) { \
    time_t tNow; time(&tNow);  struct tm* lpNow = localtime(&tNow); \
    fprintf(stdout, COLOR_RED); \
    fprintf(stdout, "[FAILURE %02d:%02d:%02d] ", lpNow->tm_hour, lpNow->tm_min, lpNow->tm_sec); \
    fprintf(stdout, __VA_ARGS__); \
    fprintf(stdout, "\n"); \
    fprintf(stdout, COLOR_RESET); \
    fflush(stdout); \
}

#define PRINT_FAILURE_AND_ABORT(...) { \
    PRINT_FAILURE(##__VA_ARGS__); \
    abort(); \
}

#define PRINT_VERBOSE(...) { \
    if (ENABLE_VERBOSE_PRINT_STATEMENTS) { \
        time_t tNow; time(&tNow);  struct tm* lpNow = localtime(&tNow); \
        fprintf(stdout, "[VERBOSE %02d:%02d:%02d] ", lpNow->tm_hour, lpNow->tm_min, lpNow->tm_sec); \
        fprintf(stdout, __VA_ARGS__); \
        fprintf(stdout, "\n"); \
        fflush(stdout); \
    } \
}

/**
 * Print a banner showing `PosidepLdr`.
 */
void PrintBanner() {
    puts("");
    puts(" /$$$$$$$  /$$   /$$     /$$               /$$                   /$$                        ");
    puts("| $$__  $$|__/  | $$    | $$              | $$                  | $$                        ");
    puts("| $$  \\ $$ /$$ /$$$$$$ /$$$$$$    /$$$$$$ | $$$$$$$  /$$   /$$ /$$$$$$    /$$$$$$   /$$$$$$$");
    puts("| $$  | $$| $$|_  $$_/|_  $$_/   /$$__  $$| $$__  $$| $$  | $$|_  $$_/   /$$__  $$ /$$_____/");
    puts("| $$  | $$| $$  | $$    | $$    | $$  \\ $$| $$  \\ $$| $$  | $$  | $$    | $$$$$$$$|  $$$$$$ ");
    puts("| $$  | $$| $$  | $$ /$$| $$ /$$| $$  | $$| $$  | $$| $$  | $$  | $$ /$$| $$_____/ \\____  $$");
    puts("| $$$$$$$/| $$  |  $$$$/|  $$$$/|  $$$$$$/| $$$$$$$/|  $$$$$$$  |  $$$$/|  $$$$$$$ /$$$$$$$/");
    puts("|_______/ |__/   \\___/   \\___/   \\______/ |_______/  \\____  $$   \\___/   \\_______/|_______/ ");
    puts("                                                     /$$  | $$                              ");
    puts("DittobytesLdr. Copyright (c) 2025 Tijme Gommers.    |  $$$$$$/                              ");
    puts("Licended under Mozilla Public License (MPL)-2.0.     \\______/                               ");
    puts("");
}

/**
 * Print all available command line options.
 * 
 * @param int dwArgumentCount Amount of arguments in lpArguments.
 * @param char** lpArguments Array of arguments passed to the program.
 * @param char* lpErrorMessage An optional error message to add to the usage information.
 */
void PrintUsage(int dwArgumentCount, char** lpArguments, char* lpErrorMessage) {
    if (lpErrorMessage != NULL) PRINT_FAILURE("%s", lpErrorMessage);
    PRINT("Usage: ./loader-[platform]-[arch].exe [path to shellcode]");
}

/**
 * Print given value in HEX
 * 
 * @param uint8_t* value An array of chars to print in HEX.
 * @param size_t length The amount of bytes/chars to print.
 * @param bool reverse Reverse the output (e.g. for a pointer).
 */
void PrintInHex(uint8_t* lpBuffer, size_t lpNumberOfBytesRead, bool reverse) {
    for(size_t i = 0; i < lpNumberOfBytesRead; i ++) {
        size_t indexCorrected = reverse ? lpNumberOfBytesRead - i - 1 : i;
        printf("%02X ", lpBuffer[indexCorrected] & 0xff);
    }

    printf("\n");
}
