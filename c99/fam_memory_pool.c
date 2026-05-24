// Flexible Array Member (FAM) Memory Pool

#include <assert.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct block {
    size_t size;
    char data[];
};

struct block* block_alloc(size_t n) {
    return malloc(sizeof(struct block) + n * sizeof(char));
}

void block_free(struct block *b) {
    return free(b);
}

#define block_data(b) \
    b->data

int main(void) {
    
    struct block *b = block_alloc(5);
    if (!b) return 1;

    // offset
    size_t expected = offsetof(struct block, data);
    size_t actual = (char*)b->data - (char*)b;
    assert(expected == actual);
    printf("Offset checked: offsetof = %zu, actual = %zu\n", expected, actual);

    // writing
    memcpy(b->data, "Hello FAM!", 10);
    b->size = 10;
    printf("Stored value: %.*s\n", (int)b->size, b->data);

    block_free(b);

    // typed access
    b = block_alloc(3 * sizeof(int));
    assert(b);
    // arr[0] = 10, arr[1] = 20, arr[2] = 30;
    memcpy(block_data(b), (int[]){10, 20, 30}, 3 * sizeof(int));
    int *arr = (int*)block_data(b);
    printf("Integer Array: [%d, %d, %d]\n", arr[0], arr[1], arr[2]);
    block_free(b);

    return 0;
}