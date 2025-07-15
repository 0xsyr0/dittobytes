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
 * Windows API.
 * 
 * Contains declarations for all of the functions, macro's & data types in the Windows API.
 * https://docs.microsoft.com/en-us/previous-versions//aa383749(v=vs.85)?redirectedfrom=MSDN
 */
#include <windows.h>

/**
 * The Beacon Object File (BOF) header.
 */
#include "beacon.h"

/**
 * Define WinExec (resolved by COFF loader)
 */
DECLSPEC_IMPORT UINT WINAPI KERNEL32$WinExec(LPCSTR, UINT);
#define WinExec KERNEL32$WinExec

/**
 * The main function of your Beacon Object File (BOF). Do not change the name
 * of the `EntryFunction` to `go` (this will be automatically done).
 * 
 * Compile using `make beacon-bof-win-amd64`
 * 
 * @param char* args The arguments passed by the COFF loader.
 * @param char* alen The lenght of the arguments passed by the COFF loader.
 */
void EntryFunction(char* args, int alen) {
    BeaconPrintf(CALLBACK_OUTPUT, "Example BOF!\n");

    WinExec("calc.exe", SW_SHOW);
}
