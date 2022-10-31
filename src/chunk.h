#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
  OP_CONSTANT,
  OP_RETURN,
} OpCode;

// With the following three functions, constitutes a dynamic array of instruction codes
typedef struct {
  int count;
  int capacity;
  uint8_t* code;
  ValueArray constants;
} Chunk;

void initChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte);
// returns the index of the newly-added value in the chunk's constant array
int addConstant(Chunk* chunk, Value value);
void freeChunk(Chunk* chunk);

#endif
