#ifndef clox_debug_h
#define clox_debug_h

#include "chunk.h"

void disassembleChunk(Chunk *chunk, const char *name);

// Disassembles the instruction starting at the given offset and returns the
// offset for the following instruction
int disassembleInstruction(Chunk *chunk, int offset);

#endif
