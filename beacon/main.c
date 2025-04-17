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
 * Define the EntryFunction that MUST be the first function.
 * Do NOT modify this function or its location.
 * 
 * @return int A return value of your shellcode.
 */
int MainShellcode();
int EntryFunction() {
    return MainShellcode();
}

/**
 * Windows demo
 */    
#ifdef __WINDOWS__
#include "demo/windows.c"    
#endif

/**
 * Windows demo
 */    
#ifdef __LINUX__
#include "demo/linux.c"    
#endif

/**
 * Windows demo
 */    
#ifdef __MACOS__
#include "demo/macos.c"    
#endif

/**
 * The main function of your shellcode.
 * 
 * @return int A return value of your shellcode.
 */
int MainShellcode() {

    // As example on Windows, we pop a message box and calculator
    #ifdef __WINDOWS__
        WindowsExample();
    #endif

    // As example on Windows, we pop a message box and calculator
    #ifdef __LINUX__
        LinuxExample();
    #endif

    // As example on Windows, we pop a message box and calculator
    #ifdef __MACOS__
        MacOsExample();
    #endif

    return 0xDEADBEEF;
}
