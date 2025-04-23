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
    char* d = "QUX";

    queue_t* queue;
    if((queue = queue_init()) == NULL)
    {
        perror("error on init");
        exit(errno);
    }

    if(!queue_add(queue, a))
    {
        perror("error on add");
        exit(errno);
    }

    if(!queue_add_many(queue, b, c, d))
    {
        perror("error on add_many");
        exit(errno);
    }

    char* i;
    if((i = (char*) queue_dequeue(queue)) == NULL)
    {
        perror("error on dequeue");
        exit(errno);
    }

    printf("EXPECT : foo , GET : %s\n", i);

   if((i = (char*) queue_dequeue(queue)) == NULL)
    {
        perror("error on dequeue");
        exit(errno);
    } 
    printf("EXPECT : bar , GET : %s\n", i);

    if(!queue_free(queue))
    {
        perror("error in free");
        exit(errno);
    }
    
    return 0;
}
