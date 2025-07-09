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

class ExampleClass {

private:
    char SomeString[4] = "ABC";
    uint32_t NumberOne = 1;

public:
    uint32_t NumberTwo = 2;
  
    /**
     * Return the sum of different variable values in this class.
     * 
     * @return uint32_t The sum of different variable values in this class.
     */
    uint32_t TestGlobalsInClass() {
        return NumberOne + NumberTwo + static_cast<uint32_t>(SomeString[1]);
    }
};

/**
 * The main function of the code to test.
 * 
 * This test focusses on ensuring that globals in c++ classes still work.
 * 
 *            OS     Arch     Metamorphication                  Test                              Argument(s)                      Description
 * @verify    all    all      randomize_register_allocation     metamorphications_not_original    None                             Must be the case without metamorphications.
 * @verify    all    all      transpiled_1                      metamorphications_not_original    None                             Must be the case without metamorphications.
 * @verify    all    all      transpiled_2                      metamorphications_not_original    None                             Must be the case without metamorphications.
 * @verify    all    all      all                               returns                           uint64_t,70                      Must be the case for all compiles.
 */
extern "C" uint64_t EntryFunction() {
    ExampleClass example;
    uint64_t result = 1;

    result += example.TestGlobalsInClass();

    return result;
}