//chunk here is referred to a sequence of bytecode, op code is operation code for each each instruction in bytecode
#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

// here enum is used to assign value to the opcode, for example here value of op_return is 0 and if more is added like op_return2 it will be 1, smthing like that

// we use it in the form suppose for MOV then , OpCode MOV = op_mov if op_mov is there in enum and it has a some value due to enum;
typedef enum
{
    OP_CONSTANT,   //this is opcode to load a constant like 1,2,3..... since we dont know which constant to load, this opcode also has a operand to specify what constant to load
    OP_NIL,
    OP_TRUE,
    OP_FALSE,
    OP_POP,
    OP_GET_GLOBAL,
    OP_DEFINE_GLOBAL,
    OP_SET_GLOBAL,
    OP_EQUAL,
    OP_GREATER,
    OP_LESS,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NOT,
    OP_NEGATE,      //to negate a given number
    OP_PRINT,
    OP_RETURN,
}OpCode;

//chunk is basically creating a array where count is number of elements and capacity is the total space and code is a pointer to the array, uint8_t cuz arr cant be used
typedef struct 
{
    int count;
    int capacity;
    uint8_t* code;
    int* lines; //this is used to find which line of the code it is in the script for error or other things
    ValueArray constants;   // we are adding the value array here to create a constant pool where we can store values of operations
}Chunk;

//now to make chunk a dynamic array we need to tweak it if count = capacity so we double the capacity, create new array copy the old one and del it and add a new element this is how a dynamic array works

void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk,uint8_t byte,int line);
int addConstant(Chunk* chunk,Value value);


#endif