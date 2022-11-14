#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "table.h"
#include "value.h"

#define TABLE_MAX_LOAD 0.75

void initTable(Table *table) {
  table->count = 0;
  table->capacity = 0;
  table->entries = NULL;
}

void freeTable(Table *table) {
  FREE_ARRAY(Entry, table->entries, table->capacity);
  initTable(table);
}

/**
 * Returns the Entry corresponding to the given key in the table;
 * if the Entry's key is NULL, then entry is empty and the key was
 * not present in the table. If the caller then assigns a non-NULL
 * key to the Entry, then the key is considered added to the table
 * (caller should obviously set the value, too).
 *
 * Uses simple linear probing/open addressing, i.e. if a bucket
 * is taken, we simply locate the next empty one.
 * Only gauranteed to terminate if the table isn't completely
 * full, meaning you should always adjust the capacity to
 * TABLE_MAX_LOAD before calling this.
 */
Entry *findEntry(Entry *entries, int capacity, ObjString *key) {
  uint32_t index = key->hash % capacity;
  Entry *tombstone = NULL;

  for (;;) {
    Entry *entry = &entries[index];
    if (entry->key == NULL) {
      if (IS_NIL(entry->value)) {
        // found empty entry; return first-found tombstone, if one was found
        return tombstone != NULL ? tombstone : entry;
      } else {
        // found tombstone (null key, non-null value)
        if (tombstone == NULL) {
          tombstone = entry;
        }
      }
    } else if (entry->key == key) {
      // found the key
      return entry;
    }

    // collision; go to next entry
    index = (index + 1) % capacity;
  }
}

bool tableGet(Table *table, ObjString *key, Value *value) {
  if (table->count == 0) {
    return false;
  }

  Entry *entry = findEntry(table->entries, table->capacity, key);
  if (entry->key == NULL) {
    return false;
  }

  *value = entry->value;
  return true;
}

static void adjustCapacity(Table *table, int capacity) {
  // 1. allocate new entry table initialized to empty Entries
  Entry *entries = ALLOCATE(Entry, capacity);
  for (int i = 0; i < capacity; i++) {
    entries[i].key = NULL;
    entries[i].value = NIL_VAL;
  }

  // 2. re-insert the previous table entries into the new table entries,
  // since indices are dependent on the capacity. We don't copy tombstones,
  // so we have to re-calculate table->count
  table->count = 0;
  for (int i = 0; i < table->capacity; i++) {
    Entry *entry = &table->entries[i];
    if (entry->key == NULL) {
      continue;
    }

    Entry *dest = findEntry(entries, capacity, entry->key);
    dest->key = entry->key;
    dest->value = entry->value;
    table->count++;
  }

  // 3. free the old table entries and save the new ones in the table
  FREE_ARRAY(Entry, table->entries, table->capacity);
  table->entries = entries;
  table->capacity = capacity;
}

bool tableSet(Table *table, ObjString *key, Value value) {
  if (table->count + 1 > table->capacity * TABLE_MAX_LOAD) {
    int capacity = GROW_CAPACITY(table->capacity);
    adjustCapacity(table, capacity);
  }

  Entry *entry = findEntry(table->entries, table->capacity, key);
  bool isNewKey = entry->key == NULL;

  if (isNewKey && IS_NIL(entry->value)) {
    table->count++;
  }

  entry->key = key;
  entry->value = value;
  return isNewKey;
}

bool tableDelete(Table *table, ObjString *key) {
  if (table->capacity == 0) {
    return false;
  }

  Entry *entry = findEntry(table->entries, table->capacity, key);
  if (entry->key == NULL) {
    return false;
  }

  // place tombstone in entries array so that we know to continue
  // a linear prob across the index in future calls to findEntry
  entry->key = NULL;
  entry->value = BOOL_VAL(true);
  return true;
}

void tableAddAll(Table *from, Table *to) {
  // TODO: would be more efficient to set to's capacity at the start
  for (int i = 0; i < from->capacity; i++) {
    Entry *entry = &from->entries[i];
    if (entry->key != NULL) {
      tableSet(to, entry->key, entry->value);
    }
  }
}
