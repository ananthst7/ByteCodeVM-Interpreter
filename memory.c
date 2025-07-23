#include <stdlib.h>

#include "memory.h"

//reallocate frees if newzie is 0, if newsize<oldsize then realloc decreases the size of org array and returns the same pointer, if newsize>oldsize it grows the size if memory after
// that isnt in use, if it is in use, then it gens new block of size newsize and copies the old bytes from org array frees it and returns a new pointer. 

void* reallocate(void* pointer,size_t oldsize, size_t newsize)
{
    if(newsize==0)
    {
        free(pointer);
        return NULL;
    }
    void* result = realloc(pointer,newsize);
    if(result == NULL) exit(1);
    return result;
}
