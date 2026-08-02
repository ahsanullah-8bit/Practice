/*
    String Duplicator with strdup Emulation

    Features: malloc, memcpy, strlen.
    Goal: A function char *my_strdup(const char *src) that duplicates a string using C99 features (no VLAs in the return).
    Plan:

    · Write my_strdup using malloc + memcpy.
    · Add tests for edge cases: empty string, very long string, allocation failure handling.
    · Compare with a version that uses VLA (if the compiler allows) and discuss pros/cons.

    Notes:
        https://en.cppreference.com/c/memory/malloc
        https://en.cppreference.com/c/experimental/dynamic/strdup

        * For the last test case (Allocation Failure Handling), you've to pass the --wrap=malloc flag
            to the compiler and it'll use the __wrap_malloc function to allocate.
            gcc string_duplicator.c -o string_duplicator  -Wl,--wrap=malloc

        * Pros/Const of non-VLA vs VLA version:
            * VLAs are allocated on the stack, which is faster than heap.
            * But that doesn't apply here, as we're not told whether a pre-allocated VLA should be
                taken as a parameter.
            * The VLA version allocates on the stack, which is very limited to allocate.
            * It does stack-use-after-return, because VLAs are allocated on the stack and this one
                is local to the function, compared to the heap memory returned by my_strdup and
                gives the responsibility to the user.
*/

#include <stddef.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *__wrap_malloc(size_t size) {
    return NULL;
}

char *my_strdup(const char *src) {
    const size_t size = strlen(src) + 1;
    char *res = malloc(size);
    if (!res) {
        errno = ENOMEM;
        return NULL;
    }

    return memcpy(res, src, size);
}

char *my_strdup_vla(const char *src) {
    const size_t size = strlen(src) + 1;
    char res[size];
    return memcpy(res, src, size);
}

int main(void) {
    // Empty string
    char *empty_str = my_strdup("");
    printf("Empty string duplicate `%s`\n", empty_str);
    free(empty_str);

    // Very long string
    char *long_str = my_strdup("Hello, this a function `char *my_strdup(const char *str)` testing, which duplicates a string using C99 features and doesn't use VLAs in return.");
    printf("Very long string duplicate: `%s`\n", long_str);
    free(long_str);

    // Allocation Failure
    // You should get a `(null)` in both previous tests. You just need to compile it as described
    //  in the notes

    // Testing VLA version
    char *vla_version = my_strdup_vla("Hello VLA!");
    printf("Result from the VLA version: `%s`\n", vla_version);

    return 0;
}