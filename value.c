#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "value.h"
#include "memory.h"
#include "object.h"

void initValueArray(ValueArray* array)
{
    array->capacity = 0;
    array->count = 0;
    array->value = NULL;
}


void writeValueArray(ValueArray* array, Value value)
{
    if(array->capacity < array->count + 1)
    {
        int oldcapacity = array->capacity;
        array->capacity = GROW_CAPACITY(oldcapacity);
        array->value = GROW_ARRAY(Value,array->value,oldcapacity,array->capacity);
    }

    array->value[array->count] = value;
    array->count++;
}

void freeValueArray(ValueArray* array)
{
    FREE_ARRAY(Value,array,array->count);
    initValueArray(array);
}

void printValue(Value value)
{
    switch(value.type){

        case VAL_NIL : printf("nil"); break;
        case VAL_BOOL : printf(AS_BOOL(value) ? "true" : "false"); break;
        case VAL_NUMBER : printf("%g", AS_NUMBER(value)); break;
        case VAL_OBJ : printObject(value); break;
    
    }
    
}

bool valuesEqual(Value a, Value b){

    if(a.type != b.type) return false;
    switch(a.type){
        case VAL_BOOL : return AS_BOOL(a) == AS_BOOL(b);
        case VAL_NIL  : return true;
        case VAL_NUMBER : return AS_NUMBER(a) == AS_NUMBER(b);
        case VAL_OBJ :  return AS_OBJ(a) == AS_OBJ(b);         

        default: return false; //unreachable
    }
}