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
 * Calculates factorial of a number using iteration.
 * 
 * @param uint64_t n The number to calculate factorial for.
 * @return uint64_t The factorial of n.
 */
uint64_t FactorialIterative(uint64_t n);

/**
 * Calculates factorial of a number using recursion.
 * 
 * @param uint64_t n The number to calculate factorial for.
 * @return uint64_t The factorial of n.
 */
uint64_t FactorialRecursive(uint64_t n);

/**
 * Calculates the nth Fibonacci number using iteration.
 * 
 * @param uint64_t n The position in the Fibonacci sequence.
 * @return uint64_t The nth Fibonacci number.
 */
uint64_t FibonacciIterative(uint64_t n);

/**
 * Calculates the nth Fibonacci number using recursion.
 * 
 * @param uint64_t n The position in the Fibonacci sequence.
 * @return uint64_t The nth Fibonacci number.
 */
uint64_t FibonacciRecursive(uint64_t n);

/**
 * Calculates the Greatest Common Divisor (GCD) of two numbers using Euclidean algorithm.
 * 
 * @param uint64_t a First number.
 * @param uint64_t b Second number.
 * @return uint64_t The GCD of a and b.
 */
uint64_t GCD(uint64_t a, uint64_t b);

/**
 * Checks if a number is prime.
 * 
 * @param uint64_t n Number to check.
 * @return uint64_t 1 if prime, 0 if not prime.
 */
uint64_t IsPrime(uint64_t n);

/**
 * Get the character at a dynamic index from "Goodbye Globe!".
 * 
 * @param int index The index to retrieve.
 * @return uint8_t The value at the given index.
 */
uint8_t GetCharAtIndex(int index);

/**
 * Get a specific character from a hard-coded string.
 * 
 * @param uint64_t index The position of the character to retrieve.
 * @return uint64_t The ASCII value of the character.
 */
uint64_t GetStringCharacter(uint64_t index);

/**
 * Sum all digits in a number.
 * 
 * @param uint64_t n The number to sum digits from.
 * @return uint64_t The sum of all digits.
 */
uint64_t SumOfDigits(uint64_t n);

/**
 * Reverse an integer value.
 * 
 * @param uint64_t n The number to reverse.
 * @return uint64_t The reversed number.
 */
uint64_t ReverseInteger(uint64_t n);

/**
 * Perform binary search on a sorted array.
 * 
 * @param uint64_t target The value to search for.
 * @return uint64_t The index if found, or 0 if not found.
 */
uint64_t BinarySearch(uint64_t target);

/**
 * Swap two integers in an array.
 * 
 * @param uint64_t *a Pointer to first integer.
 * @param uint64_t *b Pointer to second integer.
 */
void Swap(uint64_t *a, uint64_t *b);

/**
 * Implement bubble sort algorithm on a fixed array and return the sum.
 * 
 * @return uint64_t Sum of sorted array.
 */
uint64_t BubbleSort();

/**
 * Helper function for QuickSort to partition the array.
 * 
 * @param uint64_t arr[] The array to partition.
 * @param uint64_t low Starting index.
 * @param uint64_t high Ending index.
 * @return uint64_t Partition index.
 */
uint64_t Partition(uint64_t arr[], uint64_t low, uint64_t high);

/**
 * Recursive implementation of QuickSort.
 * 
 * @param uint64_t arr[] The array to sort.
 * @param uint64_t low Starting index.
 * @param uint64_t high Ending index.
 */
void QuickSort(uint64_t arr[], uint64_t low, uint64_t high);

/**
 * Test the QuickSort algorithm on a fixed array and return the sum.
 * 
 * @return uint64_t Sum of sorted array.
 */
uint64_t QuickSortTest();

/**
 * Merges two subarrays of arr[].
 * 
 * @param uint64_t arr[] Array to merge.
 * @param uint64_t l Left index.
 * @param uint64_t m Middle index.
 * @param uint64_t r Right index.
 */
void Merge(uint64_t arr[], uint64_t l, uint64_t m, uint64_t r);

/**
 * Main function that sorts arr[l..r] using merge().
 * 
 * @param uint64_t arr[] Array to sort.
 * @param uint64_t l Left index.
 * @param uint64_t r Right index.
 */
void MergeSortRecursive(uint64_t arr[], uint64_t l, uint64_t r);

/**
 * Test the MergeSort algorithm on a fixed array and return the sum.
 * 
 * @return uint64_t Sum of sorted array.
 */
uint64_t MergeSort();

/**
 * Count the number of set bits in an integer (Hamming weight).
 * 
 * @param uint64_t n The number to count bits in.
 * @return uint64_t Number of set bits.
 */
uint64_t CountSetBits(uint64_t n);

/**
 * Implementation of the Ackermann function, a classic example of a 
 * non-primitive recursive function.
 * 
 * @param uint64_t m First parameter.
 * @param uint64_t n Second parameter.
 * @return uint64_t Result of the Ackermann function.
 */
uint64_t AckermanFunction(uint64_t m, uint64_t n);

/**
 * Calculate a value in Pascal's Triangle.
 * 
 * @param uint64_t row Row number (0-indexed).
 * @param uint64_t col Column number (0-indexed).
 * @return uint64_t Value at the specified position.
 */
uint64_t PascalTriangle(uint64_t row, uint64_t col);

/**
 * Test Pascal's Triangle calculation and return sum of a row.
 * 
 * @param uint64_t row Row to calculate (0-indexed).
 * @return uint64_t Sum of values in the row.
 */
uint64_t TestPascalTriangle(uint64_t row);

/**
 * Implement a Linear Congruential Generator for pseudo-random numbers.
 * 
 * @param uint64_t iterations Number of iterations.
 * @return uint64_t Sum of generated numbers.
 */
uint64_t LinearCongruentialGenerator(uint64_t iterations);

/**
 * Test implementation of the 0/1 Knapsack problem using dynamic programming.
 * 
 * @return uint64_t Maximum value that can be put in knapsack.
 */
uint64_t TestKnapsackProblem();

/**
 * Helper function for matrix multiplication.
 * 
 * @param uint64_t A[3][3] First matrix.
 * @param uint64_t B[3][3] Second matrix.
 * @param uint64_t C[3][3] Result matrix.
 */
void MultiplyMatrix(uint64_t A[3][3], uint64_t B[3][3], uint64_t C[3][3]);

/**
 * Test matrix multiplication and return sum of resulting matrix elements.
 * 
 * @return uint64_t Sum of all elements in the result matrix.
 */
uint64_t TestMatrixMultiplication();

/**
 * The main function of the code to test.
 * 
 * This test focusses on ensuring that complex code still functions. It's goal is not so much to check whether
 * certain specific transpilations work correclty.
 * 
 *            OS     Arch     Metamorphication        Test                              Argument(s)                      Description
 * @verify    all    all      all                     returns                           uint64_t,18154888378             Must be the case without metamorphications.
 */
uint64_t EntryFunction() {
    uint64_t result = 0;
    
    // Call all test functions and sum their results
    result += FactorialIterative(12);
    result += FactorialRecursive(10);
    result += FibonacciIterative(25);
    result += FibonacciRecursive(15);
    result += GCD(1071, 462);
    result += IsPrime(997) ? 1 : 0;
    result += GetStringCharacter(6);
    result += SumOfDigits(9876543210);
    result += ReverseInteger(123456789);
    result += BinarySearch(15);
    result += BubbleSort();
    result += QuickSortTest();
    result += MergeSort();
    result += CountSetBits(0xDEADBEEF);
    result += AckermanFunction(3, 4);
    result += TestPascalTriangle(12);
    result += LinearCongruentialGenerator(10);
    result += TestKnapsackProblem();
    result += TestMatrixMultiplication();
    
    return result;
}

/**
 * Calculates factorial of a number using iteration.
 * 
 * @param uint64_t n The number to calculate factorial for.
 * @return uint64_t The factorial of n.
 */
uint64_t FactorialIterative(uint64_t n) {
    uint64_t result = 1;
    uint64_t i;
    
    for (i = 2; i <= n; ++i) {
        result *= i;
    }
    
    return result;
}

/**
 * Calculates factorial of a number using recursion.
 * 
 * @param uint64_t n The number to calculate factorial for.
 * @return uint64_t The factorial of n.
 */
uint64_t FactorialRecursive(uint64_t n) {
    if (n <= 1) {
        return 1;
    }
    
    return n * FactorialRecursive(n - 1);
}

/**
 * Calculates the nth Fibonacci number using iteration.
 * 
 * @param uint64_t n The position in the Fibonacci sequence.
 * @return uint64_t The nth Fibonacci number.
 */
uint64_t FibonacciIterative(uint64_t n) {
    if (n <= 1) {
        return n;
    }
    
    uint64_t a = 0;
    uint64_t b = 1;
    uint64_t c;
    uint64_t i;
    
    for (i = 2; i <= n; ++i) {
        c = a + b;
        a = b;
        b = c;
    }
    
    return b;
}

/**
 * Calculates the nth Fibonacci number using recursion.
 * 
 * @param uint64_t n The position in the Fibonacci sequence.
 * @return uint64_t The nth Fibonacci number.
 */
uint64_t FibonacciRecursive(uint64_t n) {
    if (n <= 1) {
        return n;
    }
    
    return FibonacciRecursive(n - 1) + FibonacciRecursive(n - 2);
}

/**
 * Calculates the Greatest Common Divisor (GCD) of two numbers using Euclidean algorithm.
 * 
 * @param uint64_t a First number.
 * @param uint64_t b Second number.
 * @return uint64_t The GCD of a and b.
 */
uint64_t GCD(uint64_t a, uint64_t b) {
    while (b != 0) {
        uint64_t temp = b;
        b = a % b;
        a = temp;
    }
    
    return a;
}

/**
 * Checks if a number is prime.
 * 
 * @param uint64_t n Number to check.
 * @return uint64_t 1 if prime, 0 if not prime.
 */
uint64_t IsPrime(uint64_t n) {
    if (n <= 1) {
        return 0;
    }
    if (n <= 3) {
        return 1;
    }
    if (n % 2 == 0 || n % 3 == 0) {
        return 0;
    }
    
    uint64_t i = 5;
    while (i * i <= n) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return 0;
        }
        i += 6;
    }
    
    return 1;
}

/**
 * Get the character at a dynamic index from "Goodbye Globe!".
 * 
 * @param int index The index to retrieve.
 * @return uint8_t The value at the given index.
 */
uint8_t GetCharAtIndex(int index) {
    char message[] = "Goodbye Globe!";
    
    // Calculate length and ensure index is within bounds
    int length = 0;
    while (message[length] != '\0') {
        length++;
    }
    
    // Ensure index is within bounds (use modulo for wrapping)
    index = index % length;
    if (index < 0) {
        index += length;
    }
    
    return (uint8_t) message[index];
}

/**
 * Get a specific character from a hard-coded string.
 * 
 * @param uint64_t index The position of the character to retrieve.
 * @return uint64_t The ASCII value of the character.
 */
uint64_t GetStringCharacter(uint64_t index) {
    return (uint64_t) GetCharAtIndex((int) index);
}

/**
 * Sum all digits in a number.
 * 
 * @param uint64_t n The number to sum digits from.
 * @return uint64_t The sum of all digits.
 */
uint64_t SumOfDigits(uint64_t n) {
    uint64_t sum = 0;
    
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }
    
    return sum;
}

/**
 * Reverse an integer value.
 * 
 * @param uint64_t n The number to reverse.
 * @return uint64_t The reversed number.
 */
uint64_t ReverseInteger(uint64_t n) {
    uint64_t reversed = 0;
    
    while (n > 0) {
        reversed = reversed * 10 + (n % 10);
        n /= 10;
    }
    
    return reversed;
}

/**
 * Perform binary search on a sorted array.
 * 
 * @param uint64_t target The value to search for.
 * @return uint64_t The index if found, or 0 if not found.
 */
uint64_t BinarySearch(uint64_t target) {
    // Hard-coded sorted array
    uint64_t arr[20] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39};
    uint64_t size = 20;
    
    uint64_t left = 0;
    uint64_t right = size - 1;
    
    while (left <= right) {
        uint64_t mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            return mid;
        }
        
        if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return 0;  // Not found
}

/**
 * Swap two integers in an array.
 * 
 * @param uint64_t *a Pointer to first integer.
 * @param uint64_t *b Pointer to second integer.
 */
void Swap(uint64_t *a, uint64_t *b) {
    uint64_t temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * Implement bubble sort algorithm on a fixed array and return the sum.
 * 
 * @return uint64_t Sum of sorted array.
 */
uint64_t BubbleSort() {
    uint64_t arr[10] = {64, 34, 25, 12, 22, 11, 90, 87, 43, 5};
    uint64_t n = 10;
    uint64_t i, j;
    
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                Swap(&arr[j], &arr[j + 1]);
            }
        }
    }
    
    // Return the number at index 5
    return arr[5];
}

/**
 * Helper function for QuickSort to partition the array.
 * 
 * @param uint64_t arr[] The array to partition.
 * @param uint64_t low Starting index.
 * @param uint64_t high Ending index.
 * @return uint64_t Partition index.
 */
uint64_t Partition(uint64_t arr[], uint64_t low, uint64_t high) {
    uint64_t pivot = arr[high];
    uint64_t i = low - 1;
    uint64_t j;
    
    for (j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            Swap(&arr[i], &arr[j]);
        }
    }
    
    Swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

/**
 * Recursive implementation of QuickSort.
 * 
 * @param uint64_t arr[] The array to sort.
 * @param uint64_t low Starting index.
 * @param uint64_t high Ending index.
 */
void QuickSort(uint64_t arr[], uint64_t low, uint64_t high) {
    if (low < high) {
        uint64_t pi = Partition(arr, low, high);
        
        if (pi > 0) {
            QuickSort(arr, low, pi - 1);
        }
        QuickSort(arr, pi + 1, high);
    }
}

/**
 * Test the QuickSort algorithm on a fixed array and return the sum.
 * 
 * @return uint64_t Sum of sorted array.
 */
uint64_t QuickSortTest() {
    uint64_t arr[8] = {170, 45, 75, 90, 802, 24, 2, 66};
    uint64_t n = 8;
    
    QuickSort(arr, 0, n - 1);
    
    // Return the number at index 5
    return arr[5];
}

/**
 * Merges two subarrays of arr[].
 * 
 * @param uint64_t arr[] Array to merge.
 * @param uint64_t l Left index.
 * @param uint64_t m Middle index.
 * @param uint64_t r Right index.
 */
void Merge(uint64_t arr[], uint64_t l, uint64_t m, uint64_t r) {
    uint64_t i, j, k;
    uint64_t n1 = m - l + 1;
    uint64_t n2 = r - m;
    
    // Create temp arrays
    uint64_t L[32], R[32]; // Use fixed size arrays instead of VLAs
    
    // Copy data to temp arrays L[] and R[]
    for (i = 0; i < n1; i++) {
        L[i] = arr[l + i];
    }
    for (j = 0; j < n2; j++) {
        R[j] = arr[m + 1 + j];
    }
    
    // Merge the temp arrays back into arr[l..r]
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    // Copy the remaining elements of L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    // Copy the remaining elements of R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

/**
 * Main function that sorts arr[l..r] using merge().
 * 
 * @param uint64_t arr[] Array to sort.
 * @param uint64_t l Left index.
 * @param uint64_t r Right index.
 */
void MergeSortRecursive(uint64_t arr[], uint64_t l, uint64_t r) {
    if (l < r) {
        // Same as (l+r)/2, but avoids overflow for large l and h
        uint64_t m = l + (r - l) / 2;
        
        // Sort first and second halves
        MergeSortRecursive(arr, l, m);
        MergeSortRecursive(arr, m + 1, r);
        
        Merge(arr, l, m, r);
    }
}

/**
 * Test the MergeSort algorithm on a fixed array and return the sum.
 * 
 * @return uint64_t Sum of sorted array.
 */
uint64_t MergeSort() {
    uint64_t arr[12] = {12, 11, 13, 5, 6, 7, 3, 2, 1, 9, 10, 8};
    uint64_t n = 12;
    
    MergeSortRecursive(arr, 0, n - 1);
    
    // Return the number at index 5
    return arr[5];
}

/**
 * Count the number of set bits in an integer (Hamming weight).
 * 
 * @param uint64_t n The number to count bits in.
 * @return uint64_t Number of set bits.
 */
uint64_t CountSetBits(uint64_t n) {
    uint64_t count = 0;
    
    while (n) {
        count += n & 1;
        n >>= 1;
    }
    
    return count;
}

/**
 * Implementation of the Ackermann function, a classic example of a 
 * non-primitive recursive function.
 * 
 * @param uint64_t m First parameter.
 * @param uint64_t n Second parameter.
 * @return uint64_t Result of the Ackermann function.
 */
uint64_t AckermanFunction(uint64_t m, uint64_t n) {
    if (m == 0) {
        return n + 1;
    } else if (n == 0) {
        return AckermanFunction(m - 1, 1);
    } else {
        return AckermanFunction(m - 1, AckermanFunction(m, n - 1));
    }
}

/**
 * Calculate a value in Pascal's Triangle.
 * 
 * @param uint64_t row Row number (0-indexed).
 * @param uint64_t col Column number (0-indexed).
 * @return uint64_t Value at the specified position.
 */
uint64_t PascalTriangle(uint64_t row, uint64_t col) {
    if (col == 0 || col == row) {
        return 1;
    }
    
    return PascalTriangle(row - 1, col - 1) + PascalTriangle(row - 1, col);
}

/**
 * Test Pascal's Triangle calculation and return sum of a row.
 * 
 * @param uint64_t row Row to calculate (0-indexed).
 * @return uint64_t Sum of values in the row.
 */
uint64_t TestPascalTriangle(uint64_t row) {
    uint64_t sum = 0;
    uint64_t i;
    
    // Limit row size for performance reasons
    uint64_t actual_row = row % 13;
    
    for (i = 0; i <= actual_row; i++) {
        sum += PascalTriangle(actual_row, i);
    }
    
    return sum;
}

/**
 * Implement a Linear Congruential Generator for pseudo-random numbers.
 * 
 * @param uint64_t iterations Number of iterations.
 * @return uint64_t Sum of generated numbers.
 */
uint64_t LinearCongruentialGenerator(uint64_t iterations) {
    // LCG parameters
    const uint64_t a = 1664525;
    const uint64_t c = 1013904223;
    const uint64_t m = 0xFFFFFFFF;  // 2^32
    
    uint64_t seed = 12345;  // Initial seed
    uint64_t sum = 0;
    uint64_t i;
    
    for (i = 0; i < iterations; i++) {
        seed = (a * seed + c) % m;
        sum += seed;
    }
    
    return sum;
}

/**
 * Test implementation of the 0/1 Knapsack problem using dynamic programming.
 * 
 * @return uint64_t Maximum value that can be put in knapsack.
 */
uint64_t TestKnapsackProblem() {
    // Fixed values and weights
    uint64_t val[5] = {60, 100, 120, 80, 40};
    uint64_t wt[5] = {10, 20, 30, 15, 25};
    uint64_t c = 50;  // Knapsack capacity
    uint64_t n = 5;   // Number of items
    
    uint64_t dp[51][6] = {0};  // Use fixed size array
    uint64_t i, w;
    
    // Build table dp[][] in bottom-up manner
    for (i = 0; i <= n; i++) {
        for (w = 0; w <= c; w++) {
            if (i == 0 || w == 0) {
                dp[w][i] = 0;
            } else if (wt[i - 1] <= w) {
                uint64_t include = val[i - 1] + dp[w - wt[i - 1]][i - 1];
                uint64_t exclude = dp[w][i - 1];
                dp[w][i] = (include > exclude) ? include : exclude;
            } else {
                dp[w][i] = dp[w][i - 1];
            }
        }
    }
    
    return dp[c][n];
}

/**
 * Helper function for matrix multiplication.
 * 
 * @param uint64_t A[3][3] First matrix.
 * @param uint64_t B[3][3] Second matrix.
 * @param uint64_t C[3][3] Result matrix.
 */
void MultiplyMatrix(uint64_t A[3][3], uint64_t B[3][3], uint64_t C[3][3]) {
    uint64_t i, j, k;
    
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            C[i][j] = 0;
            for (k = 0; k < 3; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

/**
 * Test matrix multiplication and return sum of resulting matrix elements.
 * 
 * @return uint64_t Sum of all elements in the result matrix.
 */
uint64_t TestMatrixMultiplication() {
    uint64_t A[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    uint64_t B[3][3] = {{9, 8, 7}, {6, 5, 4}, {3, 2, 1}};
    uint64_t C[3][3] = {0};
    
    MultiplyMatrix(A, B, C);
    
    uint64_t sum = 0;
    uint64_t i, j;
    
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            sum += C[i][j];
        }
    }
    
    return sum;
}