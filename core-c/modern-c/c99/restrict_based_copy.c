/*
    restrict based vector copy

    Features: restrict pointers, memcpy.
    Goal: A function void safe_copy(int * restrict dst, const int * restrict src, size_t n) that copies
        an array without aliasing concerns.

    Plan:
    * Write the function declaration with restrict.
    * Measure performance (with a simple timer) when copying overlapping vs. non‑overlapping arrays;
        demonstrate that restrict allows better optimization.
    * Write a test that shows the compiler can vectorize when restrict is present.

    Notes:
    * What is `restrict` keyword?
        A contractual promise made to the compiler stating, the pointer is the only way to read or
            write to the memory it points to.
        Basically, while reading, you tell the compiler not to reload the memory of other pointers
            again upon changes to other pointers. Similarly, while writing, you tell it that changes
            to this pointer will not alter the data of other pointers.
        More on that at https://en.cppreference.com/c/language/restrict.

    * Memory Aliasing: Basically, memory pointed-to/aliased by pointers.
        See https://developer.arm.com/documentation/100748/0624/Writing-Optimized-Code/C-and-C---aliasing
            https://en.wikipedia.org/wiki/Aliasing_(computing)

    * Compiler Vectorization: Transforms scaler loop implementations to sequences of vector operations.
        See https://en.wikipedia.org/wiki/Automatic_vectorization


    * You must use this command to see vectorization being successful
        On GCC:
            gcc -O3 --param vect-max-version-for-alias-checks=0 -fopt-info-vec-optimized -fno-builtin restrict_based_copy.c -o restrict_based_copy
            ./restrict_based_copy
*/

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void start_timer(clock_t *start) {
    *start = clock();
}

double end_timer(clock_t *start) {
    clock_t end = clock();
    double seconds = (double)(end - *start) / CLOCKS_PER_SEC; // CPU ticks
    return seconds * 1000.0;
}

void safe_copy(int * restrict dst, const int * restrict src, size_t n) {
    if (!dst || !src || dst == src || n < 1)
        return;

    while (n--)
        *dst++ = *src++;
}

void basic_copy(int *dst, const int *src, size_t n) {
    if (!dst || !src || dst == src || n < 1)
        return;

    while(n--)
        *dst++ = *src++;    // both * & ++ has same precedence, but ++ will take affect after the whole statement.
}


#define ARRAY_SIZE 100000 // 100k
#define ITERATIONS 10000  // 10k

void test_overlapping(int *arr, size_t n) {
    clock_t start;
    start_timer(&start);
    for (int i = 0; i < ITERATIONS; ++i)
        safe_copy(arr, arr + 2, n / 2);
    printf("Overlapping safe copy took: %f ms.\n", end_timer(&start));

    start_timer(&start);
    for (int i = 0; i < ITERATIONS; ++i)
        basic_copy(arr, arr + 2, n / 2);
    printf("Overlapping basic copy took: %f ms.\n", end_timer(&start));
}

void test_non_overlapping(int *arr, size_t n) {
    clock_t start;
    start_timer(&start);
    for (int i = 0; i < ITERATIONS; ++i)
        safe_copy(arr, arr + n / 2, n / 2);
    printf("Non-overlapping safe copy took: %f ms\n", end_timer(&start));

    start_timer(&start);
    for (int i = 0; i < ITERATIONS; ++i)
        basic_copy(arr, arr + n / 2, n / 2);
    printf("Non-overlapping basic copy took: %f ms\n", end_timer(&start));
}

int main(void) {
    int arr[ARRAY_SIZE + 10] = {0};
    for (int i = 0; i < ARRAY_SIZE + 10; ++i)
        arr[i] = i;

    test_overlapping(arr, ARRAY_SIZE);
    test_non_overlapping(arr, ARRAY_SIZE);
}