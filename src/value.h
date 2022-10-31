#ifndef clox_value_h
#define clox_value_h

#include "common.h"

typedef double Value;

// With the following three functions, constitutes a dynamic array of Values
typedef struct {
  int capacity;
  int count;
  Value* values;
} ValueArray;

void initValueArray(ValueArray* array);
void writeValueArray(ValueArray* array, Value value);
void freeValueArray(ValueArray* array);
void printValue(Value value);

#endif
