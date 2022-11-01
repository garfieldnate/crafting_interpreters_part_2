#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "value.h"

#define STACK_MAX 256

typedef struct {
  Chunk* chunk;
  // instruction pointer; points to NEXT byte to be read
  uint8_t* ip;
  Value stack[STACK_MAX];
  // points to the next free slot in the stack (so the top element is
  // actually stack[stackTop - 1])
  Value* stackTop;
} VM;

typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR,
} InterpretResult;

void initVM();
void freeVM();
InterpretResult interpret(Chunk* chunk);

// manipulate the VM stack
void push(Value value);
Value pop();

#endif