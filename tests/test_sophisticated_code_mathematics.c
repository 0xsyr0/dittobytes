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
 * Subtract two integers.
 *
 * @param int a First integer.
 * @param int b Second integer.
 * @return int The result.
 */
int sub(int a, int b);

/**
 * Multiply two integers.
 *
 * @param int a First integer.
 * @param int b Second integer.
 * @return int The result.
 */
int mul(int a, int b);

/**
 * Divide two integers.
 *
 * @param int a Numerator.
 * @param int b Denominator.
 * @return int The result.
 */
int divide(int a, int b);

/**
 * Compute the factorial of a number.
 *
 * @param int n The number.
 * @return int The factorial.
 */
int factorial(int n);

/**
 * Check if a number is even.
 *
 * @param int n The number.
 * @return bool True if even.
 */
bool is_even(int n);

/**
 * Compute the nth Fibonacci number.
 *
 * @param int n The index.
 * @return int The number.
 */
int fibonacci(int n);

/**
 * Raise a number to a power.
 *
 * @param int base The base.
 * @param int exp The exponent.
 * @return int The result.
 */
int power(int base, int exp);

/**
 * Compute the sum of digits.
 *
 * @param int n The number.
 * @return int The digit sum.
 */
int digit_sum(int n);

/**
 * Check if a number is a palindrome.
 *
 * @param int n The number.
 * @return bool True if palindrome.
 */
bool is_palindrome(int n);

/**
 * Reverse a number.
 *
 * @param int n The number.
 * @return int The reversed number.
 */
int reverse(int n);

/**
 * Get max of two integers.
 *
 * @param int a First integer.
 * @param int b Second integer.
 * @return int The max value.
 */
int max(int a, int b);

/**
 * Get min of two integers.
 *
 * @param int a First integer.
 * @param int b Second integer.
 * @return int The min value.
 */
int min(int a, int b);

/**
 * Get the absolute value.
 *
 * @param int n The number.
 * @return int The absolute value.
 */
int absolute(int n);

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
 * Sum an array of integers.
 *
 * @param int* arr The array.
 * @param int size Number of elements.
 * @return int The total sum.
 */
int sum_array(int* arr, int size);

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
 * @return uint64_t The return value to verify: `9876543454` (which must still be the case after transpilation).
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

    return 9876543210ULL + (uint64_t)mixed;
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
 * Subtract two integers.
 *
 * @param int a First integer.
 * @param int b Second integer.
 * @return int The result.
 */
int sub(int a, int b) {
    return a - b;
}

/**
 * Multiply two integers.
 *
 * @param int a First integer.
 * @param int b Second integer.
 * @return int The result.
 */
int mul(int a, int b) {
    return a * b;
}

/**
 * Divide two integers.
 *
 * @param int a Numerator.
 * @param int b Denominator.
 * @return int The result.
 */
int divide(int a, int b) {
    return b == 0 ? 0 : a / b;
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
 * Check if a number is even.
 *
 * @param int n The number.
 * @return bool True if even.
 */
bool is_even(int n) {
    return (n % 2) == 0;
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
 * Raise a number to a power.
 *
 * @param int base The base.
 * @param int exp The exponent.
 * @return int The result.
 */
int power(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; ++i)
        result *= base;
    return result;
}

/**
 * Compute the sum of digits.
 *
 * @param int n The number.
 * @return int The digit sum.
 */
int digit_sum(int n) {
    int sum = 0;
    while (n != 0) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

/**
 * Check if a number is a palindrome.
 *
 * @param int n The number.
 * @return bool True if palindrome.
 */
bool is_palindrome(int n) {
    return n == reverse(n);
}

/**
 * Reverse a number.
 *
 * @param int n The number.
 * @return int The reversed number.
 */
int reverse(int n) {
    int rev = 0;
    while (n != 0) {
        rev = rev * 10 + n % 10;
        n /= 10;
    }
    return rev;
}

/**
 * Get max of two integers.
 *
 * @param int a First integer.
 * @param int b Second integer.
 * @return int The max value.
 */
int max(int a, int b) {
    return a > b ? a : b;
}

/**
 * Get min of two integers.
 *
 * @param int a First integer.
 * @param int b Second integer.
 * @return int The min value.
 */
int min(int a, int b) {
    return a < b ? a : b;
}

/**
 * Get the absolute value.
 *
 * @param int n The number.
 * @return int The absolute value.
 */
int absolute(int n) {
    return n < 0 ? -n : n;
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
 * Sum an array of integers.
 *
 * @param int* arr The array.
 * @param int size Number of elements.
 * @return int The total sum.
 */
int sum_array(int* arr, int size) {
    int sum = 0;
    for (int i = 0; i < size; ++i)
        sum += arr[i];
    return sum;
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
