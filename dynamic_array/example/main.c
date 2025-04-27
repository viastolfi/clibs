#define DA_LIB_IMPLEMENTATION
#include "../da.h"

#include <stdio.h>

typedef struct
{
    int* items;
    size_t count;
    size_t capacity;
} Numbers;

int main(void) 
{
    Numbers numbers = {0};    

    da_append(&numbers, 1);
    da_append(&numbers, 2);
    da_append(&numbers, 3);

    da_foreach(int, it, &numbers) {
        size_t index = it - numbers.items;
        printf("%zu: %d\n", index, *it); 
    }

    da_free(&numbers);
}
