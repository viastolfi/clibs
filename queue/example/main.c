#define QUEUE_LIB_IMPLEMENTATION
#define QUEUE_NO_THREADING
#include "../queue.h"

#include <stdio.h>
#include <errno.h>

int main(void)
{
    char* a = "foo";
    char* b = "bar";
    char* c = "baz";

    queue_t* queue;
    if((queue = queue_init()) == NULL)
    {
        perror("error on init");
        exit(errno);
    }

    if( 
        !queue_add(queue, a) ||
        !queue_add(queue, b) ||
        !queue_add(queue, c))
    {
        perror("error on add");
        exit(errno);
    }

    char* i;
    if((i = (char*) queue_dequeue(queue)) == NULL)
    {
        perror("error on dequeue");
        exit(errno);
    }

    // expected foo
    printf("value in dequeued : %s\n", i);

    if(!queue_free(queue))
    {
        perror("error in free");
        exit(errno);
    }
    
    return 0;
}
