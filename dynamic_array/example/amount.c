#define DA_LIB_IMPLEMENTATION
#include "../da.h"

#include <stdio.h>

// THIS CAN BE USED AS A STRING BUILDER
typedef struct
{
    char* items;
    size_t count;
    size_t capacity;
} Chars;

int main(void) 
{
    Chars chars = {0};    

    da_append(&chars, 'a');
    da_append_amount(&chars, 'b', 3);

    printf("%s\n", chars.items);

    da_free(&chars);
}
