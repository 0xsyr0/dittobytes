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
 * Booleans.
 * 
 * Defines boolean types.
 * https://pubs.opengroup.org/onlinepubs/007904975/basedefs/stdbool.h.html
 */
#include <stdbool.h>

/**
 * Windows API.
 * 
 * Contains declarations for all of the functions, macro's & data types in the Windows API.
 * https://docs.microsoft.com/en-us/previous-versions//aa383749(v=vs.85)?redirectedfrom=MSDN
 */
#include <windows.h>

/**
 * NTSTATUS Codes Header.
 * 
 * Definitions for NTSTATUS codes used in Windows operating systems.
 * https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-erref/596a1078-e883-4972-9bbc-49e60bebca55
 */
#include <ntstatus.h>

/**
 * Fixed size integer types.
 * 
 * Defines a number of macros to be used with printf and scanf family of functions, as well as functions for working with the intmax_t type.
 * https://pubs.opengroup.org/onlinepubs/009695399/basedefs/inttypes.h.html
 */
#include <inttypes.h>

/**
 * Custom helper functions that do not use global variables
 */
#include "../lib/console.c"
#include "../lib/error.c"

/**
 * Instruct the loader to run the shellcode file passed as first argument.
 *
 * @param int argc Amount of arguments in lpArguments.
 * @param char** argv Array of arguments passed to the program.
 * @returns int The resulting status
 */
int main(int argc, char** argv) {
    // Variable declarations
    NTSTATUS dwResult = STATUS_RETRY;
    LPVOID lpShellcode = NULL;
    HANDLE hShellcodeFile = INVALID_HANDLE_VALUE;
    DWORD dwShellcodeSize = INVALID_FILE_SIZE;
    DWORD dwShellcodeSizeRead = 0;
    DWORD lpMemoryProtection = PAGE_READWRITE;
    size_t (*lpCallable)();
    size_t dwCallableResult = 0;

    // Show banner of the simple shellcode loader
    PrintBanner();

    // Require the user to specify a path to the shellcode to load
    if (argc != 2 || strlen(argv[1]) == 0) {
        PrintUsage(argc, argv, "Please specify a shellcode file.");
        dwResult = STATUS_INVALID_PARAMETER;
        goto CLEANUP_AND_RETURN;
    } else {
        PRINT_SUCCESS("Obtained path to shellcode file: %s.", argv[1]);
    }

    // Open handle to file
    hShellcodeFile = CreateFileA(argv[1], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); 
    if (hShellcodeFile == INVALID_HANDLE_VALUE) {
        PrintMessageFromLastError("The specified shellcode file does not exist.");
        dwResult = STATUS_INVALID_PARAMETER;
        goto CLEANUP_AND_RETURN;
    } else {
        PRINT_SUCCESS("Obtained handle to shellcode file: 0x%zX.", (size_t) hShellcodeFile);
    }

    // Get file size
    dwShellcodeSize = GetFileSize(hShellcodeFile, NULL);
    if (dwShellcodeSize == INVALID_FILE_SIZE) {
        PrintMessageFromLastError("Cannot get shellcode file size.");
        dwResult = INVALID_FILE_SIZE;
        goto CLEANUP_AND_RETURN;
    } else {
        PRINT_SUCCESS("Obtained shellcode file size in bytes: %lu.", dwShellcodeSize);
    }

    // Check file size
    if (dwShellcodeSize <= 0) {
        PRINT_FAILURE("Shellcode file is empty or has an invalid size (%lu bytes).", dwShellcodeSize);
        dwResult = INVALID_FILE_SIZE;
        goto CLEANUP_AND_RETURN;
    } else {
        PRINT_SUCCESS("Shellcode contains at least one byte.");
    }

    // Allocate memory for the shellcode with RW permissions
    lpShellcode = VirtualAlloc(NULL, dwShellcodeSize, MEM_COMMIT | MEM_RESERVE, lpMemoryProtection);
    if (lpShellcode == NULL) {
        PrintMessageFromLastError("Failed to allocate memory for shellcode.");
        dwResult = STATUS_NO_MEMORY;
        goto CLEANUP_AND_RETURN;
    } else {
        PRINT_SUCCESS("Allocated memory region for shellcode (RW): %p.", lpShellcode);
    }

    // Read shellcode into allocated memory
    dwShellcodeSizeRead = 0;
    if (!ReadFile(hShellcodeFile, lpShellcode, dwShellcodeSize, &dwShellcodeSizeRead, NULL) || dwShellcodeSizeRead != dwShellcodeSize) {
        PrintMessageFromLastError("Failed to read shellcode file into memory.");
        dwResult = STATUS_IO_DEVICE_ERROR;
        goto CLEANUP_AND_RETURN;
    } else {
        PRINT_SUCCESS("Read shellcode file into allocated memory region.");
    }

    // Change shellcode memory permissions to RX
    if (!VirtualProtect(lpShellcode, dwShellcodeSize, PAGE_EXECUTE_READ, &lpMemoryProtection)) {
        PrintMessageFromLastError("Failed to change memory protection to PAGE_EXECUTE_READ.");
        dwResult = STATUS_ACCESS_DENIED;
        goto CLEANUP_AND_RETURN;
    } else {
        PRINT_SUCCESS("Changed memory protection to PAGE_EXECUTE_READ.");
    }

    // Preview & execute shellcode
    PRINT_SUCCESS("Preview of shellcode: %02X %02X %02X %02X ... %02X %02X %02X %02X.", ((uint8_t*) lpShellcode)[0], ((uint8_t*) lpShellcode)[1], ((uint8_t*) lpShellcode)[2], ((uint8_t*) lpShellcode)[3], ((uint8_t*) lpShellcode)[dwShellcodeSize - 4], ((uint8_t*) lpShellcode)[dwShellcodeSize - 3], ((uint8_t*) lpShellcode)[dwShellcodeSize - 2], ((uint8_t*) lpShellcode)[dwShellcodeSize - 1]);
    PRINT_SUCCESS("Executing the shellcode (this might take a while or crash).");
    lpCallable = (size_t (*)()) lpShellcode;
    dwCallableResult = lpCallable();
    PRINT_SUCCESS("Finished executing shellcode.");

    PRINT_SUCCESS("Got result from shellcode (int64_t):  0x%016" PRIx64 " (signed %" PRId64 ").", (int64_t) dwCallableResult, (int64_t) dwCallableResult);
    PRINT_SUCCESS("Got result from shellcode (uint64_t): 0x%016" PRIx64 " (unsigned %" PRIu64 ").", (uint64_t) dwCallableResult, (uint64_t) dwCallableResult);
    PRINT_SUCCESS("Got result from shellcode (int32_t):  0x%08" PRIx32 " (signed %" PRId32 ").", (int32_t) dwCallableResult, (int32_t) dwCallableResult);
    PRINT_SUCCESS("Got result from shellcode (uint32_t): 0x%08" PRIx32 " (unsigned %" PRIu32 ").", (uint32_t) dwCallableResult, (uint32_t) dwCallableResult);
    PRINT_SUCCESS("Got result from shellcode (int16_t):  0x%04" PRIx16 " (signed %" PRId16 ").", (int16_t) dwCallableResult, (int16_t) dwCallableResult);
    PRINT_SUCCESS("Got result from shellcode (uint16_t): 0x%04" PRIx16 " (unsigned %" PRIu16 ").", (uint16_t) dwCallableResult, (uint16_t) dwCallableResult);
    PRINT_SUCCESS("Got result from shellcode (int8_t):   0x%02" PRIx8 " (signed %" PRId8 ").", (int8_t) dwCallableResult, (int8_t) dwCallableResult);
    PRINT_SUCCESS("Got result from shellcode (uint8_t):  0x%02" PRIx8 " (unsigned %" PRIu8 ").", (uint8_t) dwCallableResult, (uint8_t) dwCallableResult);

    dwResult = STATUS_SUCCESS;
    goto CLEANUP_AND_RETURN;

CLEANUP_AND_RETURN:
    if (lpShellcode != NULL) VirtualFree(lpShellcode, 0, MEM_RELEASE);
    if (hShellcodeFile != NULL) CloseHandle(hShellcodeFile);
    hShellcodeFile = INVALID_HANDLE_VALUE;
    goto RETURN;

RETURN:
    return dwResult;
}