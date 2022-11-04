#include <stdio.h>
#include <stdlib.h>

#include "memory.h"
#include "vm.h"

// ALL allocations in clox go through this function, which lets us
// track memory usage.
// call reallocate(NULL, 0, size_t) to malloc new memory
void *reallocate(void *pointer, size_t oldSize, size_t newSize) {
  if (newSize == 0) {
    free(pointer);
    return NULL;
  }

  void *result = realloc(pointer, newSize);
  if (result == NULL) {
    printf("clox: Could not allocate memory!");
    exit(1);
  }
  return result;
}

void freeObject(Obj *object) {
  switch (object->type) {
  case OBJ_STRING: {
    ObjString *string = (ObjString *)object;
    FREE_ARRAY(char, string->chars, string->length + 1);
    FREE(ObjString, object);
    break;
  }
  }
}

void freeObjects() {
  Obj *object = vm.objects;
  while (object != NULL) {
    Obj *next = object->next;
    freeObject(object);
    object = next;
  }
}
