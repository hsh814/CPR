#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "uni_klee_memory_check.h"

// Hash map for storing graph data from file
static struct uni_klee_hash_map *uni_klee_ptr_hash_map = NULL;
static struct uni_klee_hash_map *uni_klee_base_hash_map = NULL;

static struct uni_klee_hash_map *uni_klee_hash_map_create(unsigned long long cap) {
  struct uni_klee_hash_map *hash_map = (struct uni_klee_hash_map *)malloc(sizeof(struct uni_klee_hash_map));
  hash_map->size = 0;
  hash_map->table_size = cap;
  hash_map->table = (struct uni_klee_key_value_pair **)malloc(sizeof(struct uni_klee_key_value_pair *) * cap);
  memset(hash_map->table, 0, sizeof(struct uni_klee_key_value_pair *) * cap);
  return hash_map;
}

static unsigned long long uni_klee_hash_map_fit(unsigned long long key, unsigned long long table_size) {
  return key % table_size;
}

static void uni_klee_hash_map_resize(struct uni_klee_hash_map *map) {
  unsigned long long new_table_size = map->table_size * 2;
  struct uni_klee_key_value_pair **new_table = (struct uni_klee_key_value_pair **)malloc(sizeof(struct uni_klee_key_value_pair*) * new_table_size);
  memset(new_table, 0, sizeof(struct uni_klee_key_value_pair *) * new_table_size);
  for (unsigned long long i = 0; i < map->table_size; i++) {
    struct uni_klee_key_value_pair *pair = map->table[i];
    while (pair != NULL) {
      struct uni_klee_key_value_pair *next = pair->next;
      unsigned long long new_index = uni_klee_hash_map_fit(pair->key, new_table_size);
      pair->next = new_table[new_index];
      new_table[new_index] = pair;
      pair = next;
    }
  }
  free(map->table);
  map->table = new_table;
  map->table_size = new_table_size;
}

static void uni_klee_hash_map_insert(struct uni_klee_hash_map *map, unsigned long long key, unsigned long long value) {
  unsigned long long index = uni_klee_hash_map_fit(key, map->table_size);
  struct uni_klee_key_value_pair *pair = map->table[index];
  while (pair != NULL) {
    if (pair->key == key) {
      // Replace the value if already exists
      pair->value = value;
      return;
    }
    pair = pair->next;
  }
  // Insert a new pair
  struct uni_klee_key_value_pair *new_pair = (struct uni_klee_key_value_pair *)malloc(sizeof(struct uni_klee_key_value_pair));
  new_pair->key = key;
  new_pair->value = value;
  new_pair->next = map->table[index];
  map->table[index] = new_pair;
  map->size++;
  if (map->size > map->table_size * 2) {
    uni_klee_hash_map_resize(map);
  }
}

static struct uni_klee_key_value_pair *uni_klee_hash_map_get(struct uni_klee_hash_map *map, unsigned long long key) {
  unsigned long long index = uni_klee_hash_map_fit(key, map->table_size);
  struct uni_klee_key_value_pair *pair = map->table[index];
  while (pair != NULL) {
    if (pair->key == key) {
      return pair;
    }
    pair = pair->next;
  }
  return NULL;
}

void uni_klee_hash_map_free(struct uni_klee_hash_map *map) {
  for (unsigned long long i = 0; i < map->table_size; i++) {
    struct uni_klee_key_value_pair *pair = map->table[i];
    while (pair != NULL) {
      struct uni_klee_key_value_pair *next = pair->next;
      free(pair);
      pair = next;
    }
  }
  free(map->table);
  free(map);
}

void uni_klee_heap_check(void **start_points, char **ids, int n) {
  char *result_file = getenv("UNI_KLEE_MEM_RESULT")
  if (result_file == NULL) return;
  if (uni_klee_ptr_hash_map == NULL) {
    // Initialize the hash map
    char *base_file = getenv("UNI_KLEE_MEM_BASE_FILE");
    char *mem_file = getenv("UNI_KLEE_MEM_FILE");
    if (base_file == NULL || mem_file == NULL)
      return;
    uni_klee_ptr_hash_map = uni_klee_hash_map_create(1024);
    uni_klee_base_hash_map = uni_klee_hash_map_create(1024);
    FILE *base_fp = fopen(base_file, "r");

    FILE *mem_fp = fopen(mem_file, "r");
  }

  // Build the hash map for matching the uni-klee address to actual address
  // Hash map for matching the uni-klee address to actual address
  struct uni_klee_hash_map *address_hash_map = uni_klee_hash_map_create(1024);
  for (int i = 0; i < n; i++) {
    
  }
}