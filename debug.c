#include <stdio.h>

#include "value.h"
#include "debug.h"

void disassembleChunk(Chunk* chunk,const char* name)
{
    printf("    ----%s----\n",name);

    for(int offset = 0;offset<chunk->count;)
    {
        offset = disassembleInstruction(chunk,offset);   //instead of inc offset in the forloop we inc offset in the disassembleInstruction as instructions can have diff size
    }
}

int disassembleInstruction(Chunk* chunk,int offset)
{
    printf("%04d ",offset);
    if(offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1])
    {
        printf("   | ");
    }
    else
    {
        printf("%4d ",chunk->lines[offset]);

    }

    uint8_t instruction = chunk->code[offset];
    //printf("%04d ",instruction);
    switch(instruction)
    {
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT",chunk,offset);
        
        case OP_NIL:
            return simpleInstruction("OP_NIL",offset);
        case OP_TRUE:
            return simpleInstruction("OP_TRUE",offset);
        case OP_FALSE:
            return simpleInstruction("OP_FALSE",offset);

        case OP_EQUAL:
            return simpleInstruction("OP_EQUAL",offset);
        case OP_GREATER:
            return simpleInstruction("OP_GREATER",offset);
        case OP_LESS:
            return simpleInstruction("OP_LESS",offset);
        
        case OP_ADD:
            return simpleInstruction("OP_ADD",offset);
        case OP_SUBTRACT:
            return simpleInstruction("OP_SUBTRACTION",offset);
        case OP_MULTIPLY:
            return simpleInstruction("OP_MULTIPLY",offset);
        case OP_DIVIDE:
            return simpleInstruction("OP_DIVIDE",offset);

        case OP_NOT:
            return simpleInstruction("OP_NOT",offset);
        
        case OP_NEGATE:
            return simpleInstruction("OP_NEGATE",offset);
        
        case OP_RETURN:
            return simpleInstruction("OP_RETURN",offset);
        
        default:
            printf("OpCode not found %d\n",instruction);
            return offset + 1;
    }
    
}


static int simpleInstruction(const char* name,int offset)
{
    printf("%s\n",name);
    return offset + 1;
}

static int constantInstruction(const char* name,Chunk* chunk,int offset)
{
    uint8_t constant = chunk->code[offset + 1];
    printf("%-16s%4d\n",name,constant);
    printValue(chunk->constants.value[constant]);
    printf("\n");
    return offset + 2;
}