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

#include <stdint.h>

/**
 * Windows API.
 * 
 * Contains declarations for all of the functions, macro's & data types in the Windows API.
 * https://docs.microsoft.com/en-us/previous-versions//aa383749(v=vs.85)?redirectedfrom=MSDN
 */
#if defined(__WINDOWS__)
#include <windows.h>
#endif

/**
 * Get a dynamic variable (that cannot be used by the compiler for flattening) that is NOT the number 0 to 9.
 *
 * @return void* A dynamic variable that is not 0 to 9.
 */
void* GetDynamicVariable();

/**
 * The main function of the code to test.
 *
 *            OS     Arch     Metamorphication        Test                              Argument(s)                      Description
 * @verify    all    all      transpiled_1            minimum_levenshtein_distance      transpiled_2,35                  There must be a minimum % change per compile.
 * @verify    all    all      all                     forensically_clean                None                             All compiled versions must have the minimum amount of potential forensic traces.
 * @verify    all    all      all                     returns                           uint8_t,24                       The return value must always be correct.
 */
uint8_t EntryFunction() {
    uintptr_t lpInstructionPointer = (uintptr_t) GetDynamicVariable();

    switch (lpInstructionPointer) {
        case 0: 
            return (uint8_t) 0xFEFEFEFEFEFEFE00;
            break;
        case 1: 
            return (uint8_t) 0xDEADBEEFCAFEBA01;
            break;
        case 2: 
            return (uint8_t) 0x0123456789ABCD02;
            break;
        case 3: 
            return (uint8_t) 0xAAAAAAAAAAAAAA03;
            break;
        case 4: 
            return (uint8_t) 0xBBBBBBBBBBBBBB04;
            break;
        case 5: 
            return (uint8_t) 0xCCCCCCCCCCCCCC05;
            break;
        case 6: 
            return (uint8_t) 0xDDDDDDDDDDDDDD06;
            break;
        case 7: 
            return (uint8_t) 0xEEEEEEEEEEEEEE07;
            break;
        case 8: 
        case 9: 
            return (uint8_t) 0xFFFFFFFFFFFFFF08;
            break;
        default: 
            return (uint8_t) 0x58271AD48E42B018;
            break;
    }
}

/**
 * Get a dynamic variable (that cannot be used by the compiler for flattening) that is NOT the number 0 to 9.
 *
 * @return void* A dynamic variable that is not 0 to 9.
 */
void* GetDynamicVariable() {
    void* lpDynamicVar = 0;

    #if defined(__WINDOWS__)
        #if defined(__AMD64__)            
            // PEB via GS segment
            __asm__("movq %%gs:0x60, %0" : "=r" (lpDynamicVar));
        #elif defined(__ARM64__)
            // TEB via x18 register
            __asm__("mov %0, x18" : "=r" (lpDynamicVar));
        #else
            #error "This architecture is currently unsupported for Windows"
        #endif
    #elif defined(__LINUX__)
        #if defined(__AMD64__)
            // Use FS segment base, typically TLS or TCB (Thread Control Block)
            __asm__("movq %%fs:0x0, %0" : "=r" (lpDynamicVar));
        #elif defined(__ARM64__)
            // Read Linux ARM64 Thread Pointer
            __asm__("mrs %0, tpidr_el0" : "=r" (lpDynamicVar));
        #else
            #error "This architecture is currently unsupported for Linux"
        #endif
    #elif defined(__MACOS__)
        #if defined(__AMD64__)
            // Read MacOS AMD64 Thread Pointer
            __asm__("movq %%gs:0, %0" : "=r" (lpDynamicVar));
        #elif defined(__ARM64__)
            // Read TPIDRRO_EL0 (read-only TLS base)
            __asm__("mrs %0, tpidrro_el0" : "=r" (lpDynamicVar));
        #else
            #error "This architecture is currently unsupported for MacOS"
        #endif
    #else
        #error "This platform & architecture combination is currently unsupported"
    #endif

    return lpDynamicVar;
}
