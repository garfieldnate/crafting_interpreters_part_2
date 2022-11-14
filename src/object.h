#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "value.h"

#define OBJ_TYPE(value) (AS_OBJ(value)->type)

#define IS_STRING(value) isObjType(value, OBJ_STRING)

#define AS_STRING(value) ((ObjString *)AS_OBJ(value))
#define AS_CSTRING(value) (AS_STRING(value)->chars)

typedef enum {
  OBJ_STRING,
} ObjType;

struct Obj {
  ObjType type;
  // dead-simple GC for now: all allocated objects form a linked-list we can
  // traverse and free
  struct Obj *next;
};

// TODO: would be more efficient to use a
// https://en.wikipedia.org/wiki/Flexible_array_member to avoid double realloc
// calls for string creation/destruction as well as double pointer
// indirection whenever accessing chars
struct ObjString {
  Obj obj;
  int length;
  char *chars;
  // for use as a Table key; cache for performance
  uint32_t hash;
};

ObjString *takeString(char *chars, int length);
ObjString *copyString(const char *, int length);
void printObject(Value value);

static inline bool isObjType(Value value, ObjType type) {
  return IS_OBJ(value) && OBJ_TYPE(value) == type;
}

#endif
