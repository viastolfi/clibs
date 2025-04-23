#define QUEUE_LIB_IMPLEMENTATION
#include "../queue.h"

#include <stdio.h>
#include <errno.h>

int main(void)
{
    char* a = "foo";
    char* b = "bar";
    char* c = "baz";

    queue_t* queue = queue_init(); 
    queue_add(queue, a);
    queue_add(queue, b);
    queue_add(queue, c);

    char* i = malloc(4);
    if((i = (char*) queue_dequeue(queue)) == NULL)
    {
        perror("error on dequeue");
        exit(errno);
    }

    // expected foo
    printf("value in dequeued : %s\n", i);
    return 0;
}
