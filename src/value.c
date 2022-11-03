#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "value.h"

void initValueArray(ValueArray *array) {
  array->count = 0;
  array->capacity = 0;
  array->values = NULL;
}

void writeValueArray(ValueArray *array, Value value) {
  if (array->capacity < array->count + 1) {
    int oldCapacity = array->capacity;
    array->capacity = GROW_CAPACITY(oldCapacity);
    array->values =
        GROW_ARRAY(Value, array->values, oldCapacity, array->capacity);
  }

  array->values[array->count] = value;
  array->count++;
}

void freeValueArray(ValueArray *array) {
  FREE_ARRAY(Value, array->values, array->capacity);
  initValueArray(array);
}

// TODO: take stream parameter for writing to
void printValue(Value value) {
  switch (value.type) {
  case VAL_BOOL: {
    printf(AS_BOOL(value) ? "true" : "false");
    break;
  }
  case VAL_NIL: {
    printf("nil");
    break;
  }
  case VAL_NUMBER: {
    printf("%g", AS_NUMBER(value));
    break;
  }
  case VAL_OBJ: {
    printObject(value);
    break;
  }
  }
}

// we can't use a simple memcmp() call, since there may be unitialized memory
// in a Value struct (due to both padding added by compiler and the use of a
// union)
bool valuesEqual(Value a, Value b) {
  if (a.type != b.type) {
    return false;
  }
  switch (a.type) {
  case VAL_BOOL: {
    return AS_BOOL(a) == AS_BOOL(b);
  }
  case VAL_NIL: {
    return true;
  }
  case VAL_NUMBER: {
    return AS_NUMBER(a) == AS_NUMBER(b);
  }
  case VAL_OBJ: {
    ObjString *aString = AS_STRING(a);
    ObjString *bString = AS_STRING(b);
    return aString->length == bString->length &&
           memcmp(aString->chars, bString->chars, aString->length) == 0;
  }
  // TODO: would be better to raise an error
  default:
    return false; // unreachable
  }
}
