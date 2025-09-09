#include <stdlib.h>

#include "memory.h"
#include "vm.h"


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

static void freeObject(Obj* object){
    switch(object->type){

        case OBJ_STRING:{
            ObjString* string = (ObjString*)object;
            FREE_ARRAY(char,string->chars,string->length + 1);
            FREE(ObjString, object);
            break;
        }

        case OBJ_FUNCTION: {
            ObjFunction* function = (ObjFunction*)object;
            freeChunk(&function->chunk);
            FREE(ObjFunction, object);
            break;
        }
    }
}

void freeObjects(){

    Obj* object = vm.objects;
    while(object != NULL){
        Obj* next = object->next;
        freeObject(object);
        object = next;        

    }

};
