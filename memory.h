#ifndef clox_memory_h
#define clox_memory_h

#include "common.h"

// here "\" means just continuation of define
// what #define does is it replaces whereever it sees GROW_CAP(capacity) with that expression

#define GROW_CAPACITY(capacity)\
    ((capacity) < 8 ? 8 : (capacity)*2)

// here (type*) is used to typecast the result to a pointer of a certain type
// void* just gives a generic pointer of no particular type

#define GROW_ARRAY(type,pointer,old_count,new_count)\
    (type*)reallocate(pointer,sizeof(type)*old_count,sizeof(type)*new_count)
    
void* reallocate(void* pointer,size_t oldsize,size_t newsize);

#define FREE_ARRAY(type,pointer,oldcount)\
    reallocate(pointer,sizeof(type)*oldcount,0);
    



#endif