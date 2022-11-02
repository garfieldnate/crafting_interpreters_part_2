#ifndef clox_value_h
#define clox_value_h

#include "common.h"

typedef enum {
  VAL_BOOL,
  VAL_NIL,
  VAL_NUMBER,
} ValueType;

typedef struct {
  ValueType type;
  union {
    bool boolean;
    double number;
  } as;
} Value;

// check Lox value type
#define IS_BOOL(value)    ((value).type == VAL_BOOL)
#define IS_NIL(value)     ((value).type == VAL_NIL)
#define IS_NUMBER(value)  ((value).type == VAL_NUMBER)

// convert Lox value to C type; ONLY use after checking type with above IS_* macros
#define AS_BOOL(value)    ((value).as.boolean)
#define AS_NUMBER(value)  ((value).as.number)

// convert C type to Lox value
#define BOOL_VAL(value) ((Value){VAL_BOOL, {.boolean = value}})
#define NIL_VAL       ((Value){VAL_NIL, {.number = 0}})
#define NUMBER_VAL(value)  ((Value){VAL_NUMBER, {.number = value}})

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
bool valuesEqual(Value a, Value b);

#endif
