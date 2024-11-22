#ifndef UNI_KLEE_MEMORY_CHECK_H
#define UNI_KLEE_MEMORY_CHECK_H

struct uni_klee_key_value_pair {
  unsigned long long key;
  unsigned long long value;
  struct uni_klee_key_value_pair *next;
};

struct uni_klee_hash_map {
  unsigned long long size;
  unsigned long long table_size;
  struct uni_klee_key_value_pair **table;
};

void uni_klee_heap_check(void **start_points, char **ids, int n);

#endif //UNI_KLEE_MEMORY_CHECK_H