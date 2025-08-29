#ifndef clox_debug_h
#define clox_debug_h

#include "chunk.h"

// this is for disassembling a whole chunk and it uses disassemble instruction
void disassembleChunk(Chunk* chunk,const char* name);

//this is for disassembling a single instruction
int disassembleInstruction(Chunk* chunk,int offset);

static int simpleInstruction(const char* name,int offset);

static int constantInstruction(const char* name,Chunk* chunk,int offset);

static int ByteInstruction(const char* name, Chunk* chunk, int offset);

static int jumpInstruction(const char* name, int sign, Chunk* chunk, int offset);


#endif