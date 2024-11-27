#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "uni_klee_memory_check.h"

#define UNI_LOGF(f, x... ) \
  do { \
    if (f) \
      fprintf(f, x); \
      fflush(f);     \
  } while (0)

// Hash map for storing graph data from file
static struct uni_klee_vector *uni_klee_ptr_edges = NULL;
static struct uni_klee_hash_map *uni_klee_ptr_hash_map = NULL;
static struct uni_klee_hash_map *uni_klee_base_hash_map = NULL;
static struct uni_klee_flat_map *uni_klee_start_points_map = NULL;

static struct uni_klee_vector *uni_klee_vector_create(unsigned long long cap) {
  struct uni_klee_vector *vector = (struct uni_klee_vector *)malloc(sizeof(struct uni_klee_vector));
  vector->size = 0;
  vector->capacity = cap;
  vector->data = (struct uni_klee_node *)malloc(sizeof(struct uni_klee_node) * cap);
  memset(vector->data, 0, sizeof(struct uni_klee_node) * cap);
  return vector;
}

static void uni_klee_vector_push_back(struct uni_klee_vector *vector, struct uni_klee_node node) {
  if (vector->size >= vector->capacity) {
    vector->capacity *= 2;
    vector->data = (struct uni_klee_node *)realloc(vector->data, sizeof(struct uni_klee_node) * vector->capacity);
  }
  vector->data[vector->size++] = node;
}

static struct uni_klee_node *uni_klee_vector_get(struct uni_klee_vector *vector, unsigned long long index) {
  if (index >= vector->size) {
    return NULL;
  }
  return &vector->data[index];
}

static struct uni_klee_node *uni_klee_vector_get_back(struct uni_klee_vector *vector) {
  if (vector->size == 0) {
    return NULL;
  }
  return &vector->data[vector->size - 1];
}

static void uni_klee_vector_pop(struct uni_klee_vector *vector) {
  if (vector->size == 0) {
    return;
  }
  vector->data[vector->size - 1] = (struct uni_klee_node){0, 0, 0, 0};
  vector->size--;
}

static unsigned long long uni_klee_vector_size(struct uni_klee_vector *vector) {
  return vector->size;
}

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

static void uni_klee_hash_map_insert(struct uni_klee_hash_map *map, unsigned long long key, struct uni_klee_node value) {
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
  new_pair->map = NULL;
  new_pair->next = map->table[index];
  map->table[index] = new_pair;
  map->size++;
  if (map->size > map->table_size * 2) {
    uni_klee_hash_map_resize(map);
  }
}

static void uni_klee_hash_map_insert_base(struct uni_klee_hash_map *map, unsigned long long key, struct uni_klee_node value) {
  unsigned long long index = uni_klee_hash_map_fit(key, map->table_size);
  struct uni_klee_key_value_pair *pair = map->table[index];
  while (pair != NULL) {
    if (pair->key == key) {
      UNI_LOGF(stderr, "Insert base address for the same key %llu - v %llu m %llu\n", key, pair->value.addr, pair->map);
      if (pair->map == NULL) {
        pair->value = (struct uni_klee_node){0, 0, 0, 0};
        pair->map = uni_klee_hash_map_create(8);
      }
      uni_klee_hash_map_insert(pair->map, value.addr, value);
      return;
    }
    pair = pair->next;
  }
  // Insert a new pair
  struct uni_klee_key_value_pair *new_pair = (struct uni_klee_key_value_pair *)malloc(sizeof(struct uni_klee_key_value_pair));
  new_pair->key = key;
  new_pair->value = (struct uni_klee_node){0, 0, 0, 0};
  new_pair->map = uni_klee_hash_map_create(8);
  new_pair->next = map->table[index];
  map->table[index] = new_pair;
  map->size++;
  uni_klee_hash_map_insert(new_pair->map, value.addr, value);
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

static void uni_klee_hash_map_free(struct uni_klee_hash_map *map) {
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

static struct uni_klee_flat_map *uni_klee_flat_map_create(unsigned long long size) {
  struct uni_klee_flat_map *flat_map = (struct uni_klee_flat_map *)malloc(sizeof(struct uni_klee_flat_map));
  flat_map->keys = (char **)malloc(sizeof(char *) * (size + 1));
  flat_map->values = (struct uni_klee_node *)malloc(sizeof(struct uni_klee_node) * (size + 1));
  memset(flat_map->keys, 0, sizeof(char *) * (size + 1));
  memset(flat_map->values, 0, sizeof(struct uni_klee_node) * (size + 1));
  flat_map->size = size;
  return flat_map;
}

static void uni_klee_flat_map_insert(struct uni_klee_flat_map *flat_map, char *key, struct uni_klee_node value) {
  for (unsigned long long i = 0; i < flat_map->size; i++) {
    if (flat_map->keys[i] == NULL || strcmp(flat_map->keys[i], key) == 0) {
      if (flat_map->keys[i] == NULL) {
        flat_map->keys[i] = (char *)malloc(strlen(key) + 1);
        strcpy(flat_map->keys[i], key);
      }
      flat_map->values[i] = value;
      return;
    }
  }
}

static struct uni_klee_node *uni_klee_flat_map_get(struct uni_klee_flat_map *flat_map, char *key) {
  for (unsigned long long i = 0; i < flat_map->size; i++) {
    if (flat_map->keys[i] == NULL) {
      return NULL;
    }
    fprintf(stderr, "Key %s\n", flat_map->keys[i]);
    if (strcmp(flat_map->keys[i], key) == 0) {
      return &flat_map->values[i];
    }
  }
  return NULL;
}

static void uni_klee_initialize_hash_map(int n) {
  // Initialize the hash map
  char *base_file = getenv("UNI_KLEE_MEM_BASE_FILE");
  char *mem_file = getenv("UNI_KLEE_MEM_FILE");
  if (base_file == NULL || mem_file == NULL)
    return;
  uni_klee_ptr_edges = uni_klee_vector_create(1024);
  uni_klee_ptr_hash_map = uni_klee_hash_map_create(1024);
  uni_klee_base_hash_map = uni_klee_hash_map_create(1024);
  uni_klee_start_points_map = uni_klee_flat_map_create(n);
  FILE *base_fp = fopen(base_file, "r");
  FILE *mem_fp = fopen(mem_file, "r");
  if (base_fp == NULL || mem_fp == NULL)
    return;
  size_t len = 1024;
  char line[1024];
  while (fgets(line, len, base_fp) != NULL) {
    struct uni_klee_node node = {0, 0, 0, 0};
    if(strstr(line, "[node]") != NULL) {
      sscanf(line, "[node] [addr %llu] [base %llu] [size %llu] [value %llu]",
            &node.addr, &node.base, &node.size, &node.value);
      uni_klee_hash_map_insert(uni_klee_ptr_hash_map, node.addr, node);
      uni_klee_hash_map_insert_base(uni_klee_base_hash_map, node.base, node);
    } else if(strstr(line, "[start]") != NULL){
      char id[256];
      sscanf(line, "[start] [ptr %llu] [name %[^]]]", &node.value, id);
      uni_klee_flat_map_insert(uni_klee_start_points_map, id, node);
    }
  }
  // Update memory graph for each symbolic input
  while (fgets(line, len, mem_fp) != NULL) {
    struct uni_klee_node node = {0, 0, 0, 0};
    if(strstr(line, "[node]") != NULL) {
      sscanf(line, "[node] [addr %llu] [base %llu] [size %llu] [value %llu]",
            &node.addr, &node.base, &node.size, &node.value);
      uni_klee_hash_map_insert(uni_klee_ptr_hash_map, node.addr, node);
      uni_klee_hash_map_insert_base(uni_klee_base_hash_map, node.base, node);
    }
  }
  // Generate the edge list
  for (unsigned long long i = 0; i < uni_klee_ptr_hash_map->table_size; i++) {
    struct uni_klee_key_value_pair *pair = uni_klee_ptr_hash_map->table[i];
    while (pair != NULL) {
      uni_klee_vector_push_back(uni_klee_ptr_edges, pair->value);
      pair = pair->next;
    }
  }
  // Close the file
  fclose(base_fp);
  fclose(mem_fp);
}

void uni_klee_heap_check(void **start_points, char **ids, int n) {
  char *result_file = getenv("UNI_KLEE_MEM_RESULT");
  if (result_file == NULL)
    return;
  if (uni_klee_ptr_edges == NULL) {
    uni_klee_initialize_hash_map(n);
  }
  if (uni_klee_ptr_edges == NULL || uni_klee_ptr_hash_map == NULL || uni_klee_base_hash_map == NULL || uni_klee_start_points_map == NULL)
    return;
  FILE *result_fp = fopen(result_file, "a");
  if (result_fp == NULL)
    return;
  UNI_LOGF(result_fp, "[heap-check] [begin]\n");
  // Build the hash map for matching the uni-klee address to actual address
  // Hash map for matching the uni-klee address to actual address
  // First, complete the uni-klee to real address mapping && real address to uni-klee mapping
  struct uni_klee_hash_map *u2a_hash_map = uni_klee_hash_map_create(1024);
  struct uni_klee_hash_map *a2u_hash_map = uni_klee_hash_map_create(1024);
  struct uni_klee_vector *u2a_queue = uni_klee_vector_create(16);
  for (int i = 0; i < n; i++) {
    char *id = ids[i];
    void *start_point = start_points[i];
    struct uni_klee_node *node = uni_klee_flat_map_get(uni_klee_start_points_map, id);
    UNI_LOGF(result_fp, "[heap-check] [id %s] [u-addr %llu] [a-addr %llu]\n", id, (unsigned long long)node, (unsigned long long)start_point);
    if (node == NULL) {
      continue;
    }
    void *value = NULL;
    if (start_point != NULL) { // Check if the pointer is NULL
      value = *(void **)start_point; // Read the value from the pointer
    }
    struct uni_klee_node actual_node = {(unsigned long long)start_point, 0, 0, (unsigned long long)value};
    struct uni_klee_key_value_pair *kv_pair = uni_klee_hash_map_get(u2a_hash_map, node->value);
    if (kv_pair != NULL) {
      // Address already treated
      continue;
    }
    uni_klee_hash_map_insert(u2a_hash_map, node->value, actual_node);
    uni_klee_vector_push_back(u2a_queue, (struct uni_klee_node){node->value, 0, 0, (unsigned long long)start_point});
    while (uni_klee_vector_size(u2a_queue) > 0) {
      struct uni_klee_node *node = uni_klee_vector_get_back(u2a_queue);
      unsigned long long u_addr = node->addr;
      unsigned long long u_base = node->base;
      unsigned long long a_addr = node->value;
      uni_klee_vector_pop(u2a_queue);
      struct uni_klee_key_value_pair *pair = uni_klee_hash_map_get(uni_klee_ptr_hash_map, u_addr);
      struct uni_klee_key_value_pair *base_pair = uni_klee_hash_map_get(uni_klee_base_hash_map, u_addr);
      char *a_base_ptr = (char *)a_addr;
      if (base_pair) {
        // This is base address: check all outgoing pointers in object
        unsigned long long ubase = base_pair->key;
        for (unsigned long long i = 0; i < base_pair->map->table_size; i++) {
          // key: address from uni-klee
          // map: outgoing pointers from key
          struct uni_klee_key_value_pair *pair = base_pair->map->table[i];
          while (pair != NULL) {
            struct uni_klee_key_value_pair *a_pair = uni_klee_hash_map_get(u2a_hash_map, pair->key);
            if (a_pair != NULL) {
              pair = pair->next;
              continue;
            }
            unsigned long long uoffset = pair->value.addr - ubase;
            char *a_ptr = a_base_ptr + uoffset;
            if (a_base_ptr == NULL) {
              UNI_LOGF(result_fp, "[heap-check] [error] [null-pointer] [addr %llu] [base %llu] [offset %llu]\n", pair->value.addr, ubase, uoffset);
              continue;
            }
            void *a_value = *(void **)a_ptr;
            UNI_LOGF(result_fp, "[heap-check] [mem-edge] [u-addr %llu] [u-offset %llu] [a-addr %llu] [a-value %llu]\n", pair->key, uoffset, (unsigned long long)a_ptr, (unsigned long long)a_value);
            struct uni_klee_node a_node = {(unsigned long long)a_ptr, (unsigned long long)(a_ptr - uoffset), 0, (unsigned long long)a_value};
            uni_klee_hash_map_insert(u2a_hash_map, pair->key, a_node);
            uni_klee_hash_map_insert(a2u_hash_map, (unsigned long long)a_ptr, pair->value);
            if (a_value != NULL)
              uni_klee_vector_push_back(u2a_queue, (struct uni_klee_node){pair->value.value, (unsigned long long)a_ptr, 0, (unsigned long long)a_value});
            pair = pair->next;
          }
        }
      } else {
        if (pair == NULL) {
          UNI_LOGF(result_fp, "[heap-check] [error] [no-node] [u-addr %llu] [a-addr %llu]\n", u_addr, a_addr);
          continue;
        }
        unsigned long long u_offset = u_addr - u_base;
        void *a_value = *(void **)a_addr;
        struct uni_klee_node a_node = {(unsigned long long)a_addr, (unsigned long long)(a_addr - u_offset), 0, (unsigned long long)a_value};
        UNI_LOGF(result_fp, "[heap-check] [ptr-edge] [u-addr %llu] [u-offset %llu] [a-addr %llu] [a-value %llu]\n", u_addr, u_offset, a_addr, (unsigned long long)a_value);
        uni_klee_hash_map_insert(u2a_hash_map, u_addr, a_node);
        uni_klee_hash_map_insert(a2u_hash_map, (unsigned long long)a_addr, pair->value);
        if (a_value != NULL)
          uni_klee_vector_push_back(u2a_queue, (struct uni_klee_node){pair->value.value, (unsigned long long)a_addr, 0, (unsigned long long)a_value});
      }

    }
  }
  UNI_LOGF(result_fp, "[heap-check] [end]\n");
  fclose(result_fp);
}