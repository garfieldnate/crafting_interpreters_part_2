#ifndef clox_table_h
#define clox_table_h

#include "common.h"
#include "value.h"

/**
 * key set to NULL means no value for the key is in the table;
 * key NULL and non-NULL value is a tombstone (indicating where
 * a key was deleted from the table)
 */
typedef struct {
  ObjString *key;
  Value value;
} Entry;

typedef struct {
  /**
   * Number of entries + tombstones
   */
  int count;
  int capacity;
  Entry *entries;
} Table;

void initTable(Table *table);
void freeTable(Table *table);
/**
 * If the table contains the key, returns true and sets value to the found value
 */
bool tableGet(Table *table, ObjString *key, Value *value);
bool tableSet(Table *table, ObjString *key, Value value);
bool tableDelet(Table *table, ObjString *key);
void tableAddAll(Table *from, Table *to);

#endif
