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
#include <stdbool.h>

/**
 * Add two integers.
 *
 * @param int a First integer.
 * @param int b Second integer.
 * @return int The result.
 */
int add(int a, int b);

/**
 * Compute the factorial of a number.
 *
 * @param int n The number.
 * @return int The factorial.
 */
int factorial(int n);

/**
 * Compute the nth Fibonacci number.
 *
 * @param int n The index.
 * @return int The number.
 */
int fibonacci(int n);

/**
 * Compute GCD of two integers.
 *
 * @param int a First integer.
 * @param int b Second integer.
 * @return int The GCD.
 */
int gcd(int a, int b);

/**
 * Compute LCM of two integers.
 *
 * @param int a First integer.
 * @param int b Second integer.
 * @return int The LCM.
 */
int lcm(int a, int b);

/**
 * Count number of primes below n.
 *
 * @param int n The upper limit.
 * @return int Number of primes.
 */
int count_primes(int n);

/**
 * Return a deterministic uint64_t.
 *
 *            OS     Arch     Metamorphication        Test                              Argument(s)                      Description
 * @verify    all    all      transpiled_1            minimum_levenshtein_distance      transpiled_2,30                  There must be a minimum % change per compile.
 * @verify    all    all      all                     returns                           uint64_t,9876543454              Must be the case without metamorphications.
 */
uint64_t EntryFunction() {
    int values[5] = {
        add(3, 5),
        factorial(5),
        fibonacci(10),
        lcm(12, 18),
        count_primes(100)
    };

    int mixed = 0;
    for (int i = 0; i < 5; ++i)
        mixed += values[i];

    return 9876543210ULL + (uint64_t) mixed;
}

/**
 * Add two integers.
 *
 * @param int a First integer.
 * @param int b Second integer.
 * @return int The result.
 */
int add(int a, int b) {
    return a + b;
}


/**
 * Compute the factorial of a number.
 *
 * @param int n The number.
 * @return int The factorial.
 */
int factorial(int n) {
    int result = 1;
    for (int i = 2; i <= n; ++i)
        result *= i;
    return result;
}

/**
 * Compute the nth Fibonacci number.
 *
 * @param int n The index.
 * @return int The number.
 */
int fibonacci(int n) {
    if (n <= 1) return n;
    int a = 0, b = 1, temp;
    for (int i = 2; i <= n; ++i) {
        temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

/**
 * Compute GCD of two integers.
 *
 * @param int a First integer.
 * @param int b Second integer.
 * @return int The GCD.
 */
int gcd(int a, int b) {
    while (b != 0) {
        int tmp = b;
        b = a % b;
        a = tmp;
    }
    return a;
}

/**
 * Compute LCM of two integers.
 *
 * @param int a First integer.
 * @param int b Second integer.
 * @return int The LCM.
 */
int lcm(int a, int b) {
    return a * b / gcd(a, b);
}

/**
 * Count number of primes below n.
 *
 * @param int n The upper limit.
 * @return int Number of primes.
 */
int count_primes(int n) {
    int count = 0;
    for (int i = 2; i < n; ++i) {
        bool prime = true;
        for (int j = 2; j * j <= i; ++j) {
            if (i % j == 0) {
                prime = false;
                break;
            }
        }
        if (prime) ++count;
    }
    return count;
}
