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
 * Strings.
 * 
 * Defines one variable type, one macro, and various functions for manipulating arrays of characters.
 * https://www.tutorialspoint.com/c_standard_library/string_h.htm
 */
#include <string.h>

/**
 * Errors.
 * 
 * Definition of errors that can occur while calling the API functions.
 * https://man7.org/linux/man-pages/man3/errno.3.html
 */
#include <errno.h>

/**
 * Retrieve a human-readable string describing the given error code (errno value).
 *
 * @param int iErrorCode The errno value.
 * @param char* lpBuffer The buffer to store the human-readable string.
 * @param size_t dwSize The size of the output buffer.
 * @return size_t Number of bytes written (excluding null terminator), or 0 on failure.
 */
size_t GetMessageFromError(int iErrorCode, char* lpBuffer, size_t dwSize) {
    const char* lpErrorMessage = strerror(iErrorCode);
    if (lpErrorMessage != NULL) {
        strncpy(lpBuffer, lpErrorMessage, dwSize - 1);
        lpBuffer[dwSize - 1] = '\0'; 
        return strnlen(lpBuffer, dwSize -1);
    } else {
        snprintf(lpBuffer, dwSize, "Unknown error %d", iErrorCode);
        return strnlen(lpBuffer, dwSize - 1);
    }
}

/**
 * Retrieve a human-readable string describing the last error (current errno value).
 *
 * @param char* lpBuffer The human-readable output buffer.
 * @param size_t dwSize The size of the output buffer.
 * @return size_t Number of bytes written (excluding null terminator), or 0 on failure.
 */
size_t GetMessageFromLastError(char* lpBuffer, size_t dwSize) {
    return GetMessageFromError(errno, lpBuffer, dwSize);
}

/**
 * Print a human-readable string, describing the given error code (errno), to the console.
 *
 * @param int iErrorCode The errno value to print a message for.
 * @param const char* lpMessagePrefix A custom prefix for the error message (optional).
 */
void PrintMessageFromError(int iErrorCode, const char* lpMessagePrefix) {
    if (lpMessagePrefix == NULL) lpMessagePrefix = "An error occurred.";
    
    char lpOutputBuffer[256];
    if(GetMessageFromError(iErrorCode, lpOutputBuffer, sizeof(lpOutputBuffer)) > 0) {
        PRINT_FAILURE("%s\n\t\t   0x%X: %s", lpMessagePrefix, iErrorCode, lpOutputBuffer);
    } else {
        PRINT_FAILURE("%s\n\t\t   0x%X.", lpMessagePrefix, iErrorCode);
    }
}

/**
 * Print a human-readable string, describing the last error code, to the console.
 * 
 * @param char* lpMessagePrefix A custom prefix for the error message (optional).
 */
void PrintMessageFromLastError(const char* lpMessagePrefix) {
    PrintMessageFromError(errno, lpMessagePrefix);
}
