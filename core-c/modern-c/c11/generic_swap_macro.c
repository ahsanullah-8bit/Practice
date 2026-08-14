/*
    Generic swap Macro

    Features: _Generic, static_assert, alignof.
    Goal: A macro SWAP(x, y) that swaps two variables of the same type, using _Generic to handle int, double, char* differently.
    Plan:

    · Define SWAP using _Generic to pick a size‑specific implementation.
    · Implement the underlying swap_int, swap_double, swap_str functions.
    · Test with assert that the sizes match and with actual swaps.
*/

#include <assert.h>
#include <string.h>

void swap_int(int *a, int *b) {
    if (a == b) return;

    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}

void swap_double(double *a, double *b) {
    const double temp = *a;
    *a = *b;
    *b = temp;
}

void swap_str(char **a, char **b) {
    char *temp = *a;
    *a = *b;
    *b = temp;
}

#define SWAP(x, y) _Generic((x),    \
    int*: swap_int,                 \
    double*: swap_double,           \
    char**: swap_str                \
)((x), (y))


int main(void) {
    int a = 8, b = 3;
    SWAP(&a, &b);
    assert(a == 3 && b == 8);

    double da = 3.22, db = 94.33;
    SWAP(&da, &db);
    assert(da == 94.33 && db == 3.22);

    char *str_a = "Hi", *str_b = "Hello";
    SWAP(&str_a, &str_b);
    assert(strcmp(str_a, "Hello") == 0);
    assert(strcmp(str_b, "Hi") == 0);

    return 0;
}