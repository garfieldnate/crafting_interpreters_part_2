#include <stdio.h>
#include <stdlib.h>

#include "memory.h"

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
