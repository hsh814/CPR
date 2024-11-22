#ifndef UNI_KLEE_MEMORY_CHECK_H
#define UNI_KLEE_MEMORY_CHECK_H


struct uni_klee_node {
  unsigned long long addr;
  unsigned long long base;
  unsigned long long size;
  unsigned long long value;
};

struct uni_klee_vector {
  unsigned long long size;
  unsigned long long capacity;
  struct uni_klee_node *data;
};

struct uni_klee_flat_map
{
  char **keys;
  struct uni_klee_node *values;
  unsigned long long size;
};

struct uni_klee_key_value_pair {
  unsigned long long key;
  struct uni_klee_node value;
  struct uni_klee_hash_map *map;
  struct uni_klee_key_value_pair *next;
};

struct uni_klee_hash_map {
  unsigned long long size;
  unsigned long long table_size;
  struct uni_klee_key_value_pair **table;
};

void uni_klee_heap_check(void **start_points, char **ids, int n);

#endif //UNI_KLEE_MEMORY_CHECK_H