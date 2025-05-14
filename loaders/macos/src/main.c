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
 * Miscellaneous.
 * 
 * POSIX operating system API. For open, close, read, lseek.
 * https://pubs.opengroup.org/onlinepubs/7908799/xsh/unistd.h.html
 */
#include <unistd.h>

/**
 * File Control Options.
 * 
 * File control options. For open() flags like O_RDONLY.
 * https://pubs.opengroup.org/onlinepubs/009695299/basedefs/fcntl.h.html
 */
#include <fcntl.h>

/**
 * Memory.
 * 
 * Memory management declarations. For mmap, mprotect, munmap.
 * https://pubs.opengroup.org/onlinepubs/009695399/basedefs/sys/mman.h.html
 */
#include <sys/mman.h>

/**
 * Stat.
 * 
 * Defines structure of the data returned by the functions fstat(), lstat(), and stat().
 * https://pubs.opengroup.org/onlinepubs/7908799/xsh/sysstat.h.html
 */
#include <sys/stat.h>

/**
 * Error.
 * 
 * Declaration or definition for errno.
 * https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/errno.h.html
 */
#include <errno.h>

/**
 * Strings.
 * 
 * Defines one variable type, one macro, and various functions for manipulating arrays of characters.
 * https://www.tutorialspoint.com/c_standard_library/string_h.htm
 */
#include <string.h>

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
    int iResult = EXIT_FAILURE;        // Default to failure
    void *lpShellcode = NULL;          // Pointer to allocated memory for shellcode
    int hShellcodeFile = -1;           // File descriptor for the shellcode file
    off_t iShellcodeSize = -1;         // Size of the shellcode file (off_t for large files)
    ssize_t iShellcodeSizeRead = 0;    // Bytes read by read() (ssize_t handles errors)
    size_t (*lpCallable)();            // Function pointer to execute the shellcode
    size_t iCallableResult = 0;        // Potential return value of the executed shellcode
    struct stat sFileStat;             // The size of the shellcode file

    // Show banner of the simple shellcode loader
    PrintBanner();

    // Require the user to specify a path to the shellcode to load
    if (argc != 2 || strlen(argv[1]) == 0) {
        PrintUsage(argc, argv, "Please specify a shellcode file.");
        iResult = EXIT_FAILURE;
        goto CLEANUP_AND_RETURN;
    } else {
        PRINT_SUCCESS("Obtained path to shellcode file: %s.", argv[1]);
    }

    // Open handle to file
    hShellcodeFile = open(argv[1], O_RDONLY);
    if (hShellcodeFile  < 0) {
        PrintMessageFromLastError("The specified shellcode file does not exist.");
        iResult = EXIT_FAILURE;
        goto CLEANUP_AND_RETURN;
    } else {
        PRINT_SUCCESS("Obtained handle to shellcode file: 0x%zX.", (size_t) hShellcodeFile);
    }

    // Get file size
     if (fstat(hShellcodeFile, &sFileStat) < 0) {
        PrintMessageFromLastError("Cannot get shellcode file size.");
        iResult = EXIT_FAILURE;
        goto CLEANUP_AND_RETURN;
    } else {
        iShellcodeSize = sFileStat.st_size;
        PRINT_SUCCESS("Obtained shellcode file size (in bytes): %lld.", iShellcodeSize);
    }

    // Check file size
    if (iShellcodeSize <= 0) {
        PRINT_FAILURE("Shellcode file is empty or has an invalid size (%lld bytes).", iShellcodeSize);
        iResult = EXIT_FAILURE;
        goto CLEANUP_AND_RETURN;
    } else {
        PRINT_SUCCESS("Shellcode contains at least one byte.");
    }

    // Allocate memory for the shellcode with RW permissions
    lpShellcode = mmap(NULL, iShellcodeSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (lpShellcode == MAP_FAILED) {
        PrintMessageFromLastError("Failed to allocate memory for shellcode.");
        iResult = EXIT_FAILURE;
        goto CLEANUP_AND_RETURN;
    } else {
        PRINT_SUCCESS("Allocated memory region for shellcode (RW): %p.", lpShellcode);
    }

    // Read shellcode into allocated memory
    iShellcodeSizeRead = read(hShellcodeFile, lpShellcode, iShellcodeSize);
    if (iShellcodeSizeRead != iShellcodeSize) {
        PrintMessageFromLastError("Failed to read shellcode file into memory.");
        iResult = EXIT_FAILURE;
        goto CLEANUP_AND_RETURN;
    } else {
        PRINT_SUCCESS("Read shellcode file into allocated memory region.");
    }

    // Change shellcode memory permissions to RX
    if (mprotect(lpShellcode, iShellcodeSize, PROT_READ | PROT_EXEC) < 0) {
        PrintMessageFromLastError("Failed to change memory protection to PAGE_EXECUTE_READ.");
        iResult = EXIT_FAILURE;
        goto CLEANUP_AND_RETURN;
    } else {
        PRINT_SUCCESS("Changed memory protection to PAGE_EXECUTE_READ.");
    }

    // Preview & execute shellcode
    PRINT_SUCCESS("Preview of shellcode: %02X %02X %02X %02X ... %02X %02X %02X %02X.", ((uint8_t*) lpShellcode)[0], ((uint8_t*) lpShellcode)[1], ((uint8_t*) lpShellcode)[2], ((uint8_t*) lpShellcode)[3], ((uint8_t*) lpShellcode)[iShellcodeSize - 4], ((uint8_t*) lpShellcode)[iShellcodeSize - 3], ((uint8_t*) lpShellcode)[iShellcodeSize - 2], ((uint8_t*) lpShellcode)[iShellcodeSize - 1]);
    PRINT_SUCCESS("Executing the shellcode (this might take a while or crash).");
    lpCallable = (size_t (*)()) lpShellcode;
    iCallableResult = lpCallable();
    PRINT_SUCCESS("Finished executing shellcode.");

    PRINT_SUCCESS("Got result from shellcode (int64_t):  0x%016" PRIx64 " (signed %" PRId64 ").", (int64_t) iCallableResult, (int64_t) iCallableResult);
    PRINT_SUCCESS("Got result from shellcode (uint64_t): 0x%016" PRIx64 " (unsigned %" PRIu64 ").", (uint64_t) iCallableResult, (uint64_t) iCallableResult);
    PRINT_SUCCESS("Got result from shellcode (int32_t):  0x%08" PRIx32 " (signed %" PRId32 ").", (int32_t) iCallableResult, (int32_t) iCallableResult);
    PRINT_SUCCESS("Got result from shellcode (uint32_t): 0x%08" PRIx32 " (unsigned %" PRIu32 ").", (uint32_t) iCallableResult, (uint32_t) iCallableResult);
    PRINT_SUCCESS("Got result from shellcode (int16_t):  0x%04" PRIx16 " (signed %" PRId16 ").", (int16_t) iCallableResult, (int16_t) iCallableResult);
    PRINT_SUCCESS("Got result from shellcode (uint16_t): 0x%04" PRIx16 " (unsigned %" PRIu16 ").", (uint16_t) iCallableResult, (uint16_t) iCallableResult);
    PRINT_SUCCESS("Got result from shellcode (int8_t):   0x%02" PRIx8 " (signed %" PRId8 ").", (int8_t) iCallableResult, (int8_t) iCallableResult);
    PRINT_SUCCESS("Got result from shellcode (uint8_t):  0x%02" PRIx8 " (unsigned %" PRIu8 ").", (uint8_t) iCallableResult, (uint8_t) iCallableResult);

    iResult = EXIT_SUCCESS;
    goto CLEANUP_AND_RETURN;

CLEANUP_AND_RETURN:
    if (lpShellcode != NULL && lpShellcode != MAP_FAILED) munmap(lpShellcode, iShellcodeSize);
    if (hShellcodeFile >= 0) close(hShellcodeFile);
    hShellcodeFile = -1;
    goto RETURN;

RETURN:
    return iResult;
}