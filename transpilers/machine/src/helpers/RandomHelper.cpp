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

#pragma once

/**
 * Regular includes
 */
#include <stdint.h>

/**
 * Namespace(s) to use
 */
using namespace llvm;

class RandomHelper {

public:

    static size_t getSimilarIntegerForDestination(size_t destinationSize, size_t originalValue) {
        size_t maximumValue = RandomHelper::getMaximumInteger(destinationSize);

        switch (destinationSize) {
            case 8: return getRandomInteger() & 0xFF;
            case 16: return getRandomInteger() & 0xFFFF;
            case 32: return getRandomInteger() & 0xFFFFFFFF;
            case 64: return getRandomInteger() & 0xFFFFFFFFFFFFFFFF;
            default: report_fatal_error(formatv("RandomHelper - Unknown destination size: {0}.", destinationSize));
        }
    }

private:

    static size_t getRandomInteger() {
        return (size_t) ((uint64_t) rand() << 32) | rand();
    }

    static size_t getMaximumInteger(size_t bits) {
        if (bits == 0 || bits > 64) {
            report_fatal_error(formatv("RandomHelper - Cannot get a maximum integer for bit size: {0}.", bits));
        }

        return (1ULL << bits) - 1;
    }

};