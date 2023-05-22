#include "uni_klee_runtime.h"

void klee_select_patch(int *patch_id) {
  *patch_id = 0;
}

// UNI_KLEE_START
int __cpr_choice(char* lid, char* typestr,
                     int* rvals, char** rvals_ids, int rvals_size,
                     int** lvals, char** lvals_ids, int lvals_size){
  int patch_id;
  klee_select_patch(&patch_id);
  int result;
  // REPLACE
  return result;
}
// UNI_KLEE_END

int __cpr_output(char* id, char* typestr, int value){
  return value;
}
