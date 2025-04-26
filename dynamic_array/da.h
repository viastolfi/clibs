i/*
------------------------------------------------------------------------------
DYNAMIC ARRAY LIB FOR C - ASTOLFI Vincent 2025

<!> WARNING : this lib is greatly inspired by the work of tsoding (github.com/tsoding) during his different project
<!> WARNING : My goal on this lib is just to train myself and to understand the different concept behind those da

Usage:

define your dynamic array like so :

typedef struct {
    Type* items;
    size_t count; 
    size_t capacity;
} my_dynamic_array;

------------------------------------------------------------------------------
*/
#ifndef DA_H
#define DA_h

#ifndef DA_REALLOC
#include    <stdlib.h>
#define     DA_REALLOC realloc
#endif //DA_REALLOC

#ifndef DA_FREE
#include    <stdlib.h>
#define     DA_FREE free;
#endif // DA_FREE

#ifndef DA_ASSERT
#include    <assert.h>
#define     DA_ASSERT assert;
#endif // DA_ASSSERT

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


#ifndef DA_LIB
#define     DA_LIB static inline
#endif // DA_LIB

#ifdef DA_LIB_IMPLEMENTATION

#ifndef DA_INIT_CAP 
#define     DA_INIT_CAP 256
#endif

#endif // DA_LIB_IMPLEMENTATION

#define da_append(da, item)                                                                 \
    do {                                                                                    \
        if((da)->count + 1 > (da)->capacity) {                                              \
            if((da)->capacity == 0) {                                                       \
                (da)->capacity = DA_INIT_CAP;                                               \
            } else {                                                                        \
                (da)->capacity *= 2;                                                        \
            }                                                                               \
            (da)->items = DA_REALLOC((da)->items, (da)->capacity * sizeof(*(da)->items));   \
            DA_ASSERT((da)->items != NULL && "realloc failed");                             \
        }                                                                                   \
        (da)->items[(da)->count++] = item                                                   \
    } while(0)

#define da_free(da) DA_FREE((da)->items)

#define da_foreach(Type, it, da) for(Type* it = (da)->items; it < (da)->items + (da)->count; ++it)

#define da_remove(da, i)                                                \
    do {                                                                \
        size_t j = (i);                                                 \
        DA_ASSERT(j < (da)->count && "remove : index out of bound");    \
        (da)->items[j] = (da)->items[--(da)->count];                    \
    } while(0)

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // DA_H
