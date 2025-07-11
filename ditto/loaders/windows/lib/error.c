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
 * Windows API.
 * 
 * Contains declarations for all of the functions, macro's & data types in the Windows API.
 * Define 'WIN32_LEAN_AND_MEAN' to make sure windows.h compiles without warnings.
 * https://docs.microsoft.com/en-us/previous-versions//aa383749(v=vs.85)?redirectedfrom=MSDN
 */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

/**
 * Retrieve a human-readable string describing the given error code.
 * 
 * @param DWORD dwErrorCode The error code to convert.
 * @param char* lpBuffer The human-readable output buffer.
 * @param DWORD dwSize The size of the output buffer.
 * @return DWORD dwBytesWritten The amount of bytes actually written.
 */
DWORD GetMessageFromError(DWORD dwErrorCode, char* lpBuffer, DWORD dwSize) {
    DWORD dwBytesWritten = FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
        NULL, 
        dwErrorCode, 
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
        lpBuffer, 
        dwSize, 
        NULL
    );

    if (dwBytesWritten > 0) {
        lpBuffer[dwBytesWritten -1] = 0x0; // remove newline
        return dwBytesWritten - 1;
    }

    return dwBytesWritten;
}

/**
 * Retrieve a human-readable string describing the last error code.
 * 
 * @param char* lpBuffer The human-readable output buffer.
 * @param DWORD dwSize The size of the output buffer.
 * @return DWORD dwBytesWritten The amount of bytes actually written.
 */
DWORD GetMessageFromLastError(char* lpBuffer, DWORD dwSize) {
    return GetMessageFromError(GetLastError(), lpBuffer, dwSize);
}

/**
 * Print a human-readable string, describing the given error code, to the console.
 * 
 * @param DWORD dwErrorCode The error code to print a message for.
 * @param char* lpMessagePrefix A custom prefix for the error message (optional).
 */
void PrintMessageFromError(DWORD dwErrorCode, char* lpMessagePrefix) {
    if (lpMessagePrefix == NULL) lpMessagePrefix = "An error occurred.";
    
    char lpOutputBuffer[256];
    if(GetMessageFromError(dwErrorCode, lpOutputBuffer, 256)) {
    	PRINT_FAILURE("%s\n\t\t   0x%lX: %s", lpMessagePrefix, dwErrorCode, lpOutputBuffer);
    } else {
        PRINT_FAILURE("%s\n\t\t   0x%lX.", lpMessagePrefix, dwErrorCode);
    }
}

/**
 * Print a human-readable string, describing the last error code, to the console.
 * 
 * @param char* lpMessagePrefix A custom prefix for the error message (optional).
 */
void PrintMessageFromLastError(char* lpMessagePrefix) {
    PrintMessageFromError(GetLastError(), lpMessagePrefix);
}