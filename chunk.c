#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

// initializing the values of chunk to null
void initChunk(Chunk* chunk)
{
  chunk->capacity = 0;
  chunk->count = 0;
  chunk->code = NULL;
  chunk->lines = NULL;
  initValueArray(&chunk->constants);
  
}

//write data into chunk and checking if count<capcity and dynamically increasing the array size
void writeChunk(Chunk* chunk,uint8_t byte,int line)
{
  if(chunk->capacity < chunk->count + 1)
  {
    int old_capacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(old_capacity);
    chunk->code = GROW_ARRAY(uint8_t,chunk->code,old_capacity,chunk->capacity);
    chunk->lines = GROW_ARRAY(int,chunk->lines,old_capacity,chunk->capacity);
  }
  chunk->code[chunk->count] = byte;
  chunk->lines[chunk->count] = line;
  chunk->count++;

}

void freeChunk(Chunk* chunk)
{
  FREE_ARRAY(uint8_t,chunk->code,chunk->capacity);
  FREE_ARRAY(int,chunk->lines,chunk->capacity);
  freeValueArray(&chunk->constants);
  initChunk(chunk);

}

int addConstant(Chunk* chunk,Value value)
{
  writeValueArray(&chunk->constants,value);
  return chunk->constants.count - 1;
}

