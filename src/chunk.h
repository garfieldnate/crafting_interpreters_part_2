#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
  OP_CONSTANT,

  // binary operators
  OP_ADD,
  OP_SUBTRACT,
  OP_MULTIPLY,
  OP_DIVIDE,

  // unary operators
  OP_NEGATE,

  OP_RETURN,
} OpCode;

// With the following three functions, constitutes a dynamic array of instruction codes
typedef struct {
  // count and capacity apply to both code and lines
  int count;
  int capacity;
  uint8_t* code;
  // line number corresponding to source that generated each instruction
  int* lines;
  ValueArray constants;
} Chunk;

void initChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
// returns the index of the newly-added value in the chunk's constant array
int addConstant(Chunk* chunk, Value value);
void freeChunk(Chunk* chunk);

#endif
