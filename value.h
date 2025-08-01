#ifndef clox_value_h
#define clox_value_h

#include "common.h"

typedef struct Obj Obj;
typedef struct ObjString ObjString;

typedef enum{
    VAL_BOOL,
    VAL_NIL,
    VAL_NUMBER,
    VAL_OBJ
}ValueType;

// we are gonna use only double values and it is named as value
typedef struct{
    ValueType type;
    union{
        bool boolean;
        double number;
        Obj* obj;
        
    }as;
} Value;

#define IS_BOOL(value)  ((value).type == VAL_BOOL)
#define IS_NIL(value)  ((value).type == VAL_NIL)
#define IS_NUMBER(value)  ((value).type == VAL_NUMBER)
#define IS_OBJ(value)   ((value).type == VAL_OBJ)


#define AS_BOOL(value)  ((value).as.boolean)
#define AS_NUMBER(value)  ((value).as.number)
#define AS_OBJ(value)    ((value).as.obj)

#define BOOL_VAL(value)    ((Value){VAL_BOOL,{.boolean = value}})
#define NIL_VAL            ((Value){VAL_NIL,{.number = 0}})
#define NUMBER_VAL(value)  ((Value){VAL_NUMBER, {.number = value}})
#define OBJ_VAL(object)    ((Value){VAL_OBJ,{.obj = (Obj*)object}})


// constructing a dynamic array
typedef struct 
{
    int count;
    int capacity;
    Value* value;
}ValueArray;

//same as a chunk
bool valuesEqual(Value a, Value b);
void initValueArray(ValueArray* array);
void writeValueArray(ValueArray* array,Value value);
void freeValueArray(ValueArray* array);
void printValue(Value value);



#endif