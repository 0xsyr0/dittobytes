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
 * Integers.
 * 
 * Defines macros that specify limits of integer types corresponding to types defined in other standard headers.
 * https://pubs.opengroup.org/onlinepubs/009696899/basedefs/stdint.h.html
 */
#include <stdint.h>

/**
 * In Position Independent Code (PIC) we cannot use global constants. However, we can mimic
 * global constants by using a context struct that we pass to any function that we call.
 */
struct GlobalContext {
    uint32_t a;
    uint32_t b;
};

/**
 * Example function definition.
 * 
 * @param struct GlobalContext* context The object that holds some variables that we want to use in every function.
 */
void SomeFunction(struct GlobalContext* context);

/**
 * The main function of your shellcode. 
 * 
 * @return uint32_t A return value of your shellcode.
 */
uint32_t EntryFunction() {
    // Initiate context struct (this holds all our fake 'global' variables)
    struct GlobalContext context;

    // Run a function that performs some action
    SomeFunction(&context);
    
    // Sum two variables in our global context struct
    uint32_t dwResult = context.a + context.b;

    // Define some messages
    uint8_t lpMessage[] = "Some Message";

    // Return some value
    return dwResult + lpMessage[2];
}

/**
 * Example function definition.
 * 
 * @param struct GlobalContext* context The object that holds some variables that we want to use in every function.
 */
void SomeFunction(struct GlobalContext* context) {
    context->a = 10;
    context->b = 10;
}