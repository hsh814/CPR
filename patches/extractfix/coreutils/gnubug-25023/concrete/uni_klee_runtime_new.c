#include "uni_klee_runtime.h"

int uni_klee_patch_id;

void klee_select_patch(int *patch_id) {
  *patch_id = 0;
}

void uni_klee_add_patch(int *patch_results, int patch_id, int result) {
  patch_results[patch_id] = result;
}

int uni_klee_choice(int *patch_results, int patch_id) {
  return patch_results[patch_id];
}

// UNI_KLEE_START
int __cpr_choice(char* lid, char* typestr,
                     long long* rvals, char** rvals_ids, int rvals_size,
                     int** lvals, char** lvals_ids, int lvals_size){
  // int patch_results[4096];
  int result;
  long long col_sep_length = rvals[0];
  long long constant_a;
  int patch_results[66];
  // Patch buggy # 0
  result = (1);
  uni_klee_add_patch(patch_results, 0, result);
  // Patch 1-0 # 1
  constant_a = 3;
  result = (constant_a != col_sep_length);
  uni_klee_add_patch(patch_results, 1, result);
  // Patch 2-0 # 2
  result = (col_sep_length != col_sep_length);
  uni_klee_add_patch(patch_results, 2, result);
  // Patch 3-0 # 3
  constant_a = 4;
  result = (constant_a <= col_sep_length);
  uni_klee_add_patch(patch_results, 3, result);
  // Patch 3-1 # 4
  constant_a = 5;
  result = (constant_a <= col_sep_length);
  uni_klee_add_patch(patch_results, 4, result);
  // Patch 3-2 # 5
  constant_a = 6;
  result = (constant_a <= col_sep_length);
  uni_klee_add_patch(patch_results, 5, result);
  // Patch 3-3 # 6
  constant_a = 7;
  result = (constant_a <= col_sep_length);
  uni_klee_add_patch(patch_results, 6, result);
  // Patch 3-4 # 7
  constant_a = 8;
  result = (constant_a <= col_sep_length);
  uni_klee_add_patch(patch_results, 7, result);
  // Patch 3-5 # 8
  constant_a = 9;
  result = (constant_a <= col_sep_length);
  uni_klee_add_patch(patch_results, 8, result);
  // Patch 3-6 # 9
  constant_a = 10;
  result = (constant_a <= col_sep_length);
  uni_klee_add_patch(patch_results, 9, result);
  // Patch 4-0 # 10
  constant_a = 3;
  result = (constant_a < col_sep_length);
  uni_klee_add_patch(patch_results, 10, result);
  // Patch 4-1 # 11
  constant_a = 4;
  result = (constant_a < col_sep_length);
  uni_klee_add_patch(patch_results, 11, result);
  // Patch 4-2 # 12
  constant_a = 5;
  result = (constant_a < col_sep_length);
  uni_klee_add_patch(patch_results, 12, result);
  // Patch 4-3 # 13
  constant_a = 6;
  result = (constant_a < col_sep_length);
  uni_klee_add_patch(patch_results, 13, result);
  // Patch 4-4 # 14
  constant_a = 7;
  result = (constant_a < col_sep_length);
  uni_klee_add_patch(patch_results, 14, result);
  // Patch 4-5 # 15
  constant_a = 8;
  result = (constant_a < col_sep_length);
  uni_klee_add_patch(patch_results, 15, result);
  // Patch 4-6 # 16
  constant_a = 9;
  result = (constant_a < col_sep_length);
  uni_klee_add_patch(patch_results, 16, result);
  // Patch 4-7 # 17
  constant_a = 10;
  result = (constant_a < col_sep_length);
  uni_klee_add_patch(patch_results, 17, result);
  // Patch 5-0 # 18
  constant_a = -10;
  result = (col_sep_length <= constant_a);
  uni_klee_add_patch(patch_results, 18, result);
  // Patch 5-1 # 19
  constant_a = -9;
  result = (col_sep_length <= constant_a);
  uni_klee_add_patch(patch_results, 19, result);
  // Patch 5-2 # 20
  constant_a = -8;
  result = (col_sep_length <= constant_a);
  uni_klee_add_patch(patch_results, 20, result);
  // Patch 5-3 # 21
  constant_a = -7;
  result = (col_sep_length <= constant_a);
  uni_klee_add_patch(patch_results, 21, result);
  // Patch 5-4 # 22
  constant_a = -6;
  result = (col_sep_length <= constant_a);
  uni_klee_add_patch(patch_results, 22, result);
  // Patch 5-5 # 23
  constant_a = -5;
  result = (col_sep_length <= constant_a);
  uni_klee_add_patch(patch_results, 23, result);
  // Patch 5-6 # 24
  constant_a = -4;
  result = (col_sep_length <= constant_a);
  uni_klee_add_patch(patch_results, 24, result);
  // Patch 5-7 # 25
  constant_a = -3;
  result = (col_sep_length <= constant_a);
  uni_klee_add_patch(patch_results, 25, result);
  // Patch 5-8 # 26
  constant_a = -2;
  result = (col_sep_length <= constant_a);
  uni_klee_add_patch(patch_results, 26, result);
  // Patch 5-9 # 27
  constant_a = -1;
  result = (col_sep_length <= constant_a);
  uni_klee_add_patch(patch_results, 27, result);
  // Patch 5-10 # 28
  constant_a = 0;
  result = (col_sep_length <= constant_a);
  uni_klee_add_patch(patch_results, 28, result);
  // Patch 5-11 # 29
  constant_a = 1;
  result = (col_sep_length <= constant_a);
  uni_klee_add_patch(patch_results, 29, result);
  // Patch 5-12 # 30
  constant_a = 2;
  result = (col_sep_length <= constant_a);
  uni_klee_add_patch(patch_results, 30, result);
  // Patch 6-0 # 31
  constant_a = -10;
  result = (col_sep_length < constant_a);
  uni_klee_add_patch(patch_results, 31, result);
  // Patch 6-1 # 32
  constant_a = -9;
  result = (col_sep_length < constant_a);
  uni_klee_add_patch(patch_results, 32, result);
  // Patch 6-2 # 33
  constant_a = -8;
  result = (col_sep_length < constant_a);
  uni_klee_add_patch(patch_results, 33, result);
  // Patch 6-3 # 34
  constant_a = -7;
  result = (col_sep_length < constant_a);
  uni_klee_add_patch(patch_results, 34, result);
  // Patch 6-4 # 35
  constant_a = -6;
  result = (col_sep_length < constant_a);
  uni_klee_add_patch(patch_results, 35, result);
  // Patch 6-5 # 36
  constant_a = -5;
  result = (col_sep_length < constant_a);
  uni_klee_add_patch(patch_results, 36, result);
  // Patch 6-6 # 37
  constant_a = -4;
  result = (col_sep_length < constant_a);
  uni_klee_add_patch(patch_results, 37, result);
  // Patch 6-7 # 38
  constant_a = -3;
  result = (col_sep_length < constant_a);
  uni_klee_add_patch(patch_results, 38, result);
  // Patch 6-8 # 39
  constant_a = -2;
  result = (col_sep_length < constant_a);
  uni_klee_add_patch(patch_results, 39, result);
  // Patch 6-9 # 40
  constant_a = -1;
  result = (col_sep_length < constant_a);
  uni_klee_add_patch(patch_results, 40, result);
  // Patch 6-10 # 41
  constant_a = 0;
  result = (col_sep_length < constant_a);
  uni_klee_add_patch(patch_results, 41, result);
  // Patch 6-11 # 42
  constant_a = 1;
  result = (col_sep_length < constant_a);
  uni_klee_add_patch(patch_results, 42, result);
  // Patch 6-12 # 43
  constant_a = 2;
  result = (col_sep_length < constant_a);
  uni_klee_add_patch(patch_results, 43, result);
  // Patch 6-13 # 44
  constant_a = 3;
  result = (col_sep_length < constant_a);
  uni_klee_add_patch(patch_results, 44, result);
  // Patch 7-0 # 45
  constant_a = -10;
  result = (constant_a == col_sep_length);
  uni_klee_add_patch(patch_results, 45, result);
  // Patch 7-1 # 46
  constant_a = -9;
  result = (constant_a == col_sep_length);
  uni_klee_add_patch(patch_results, 46, result);
  // Patch 7-2 # 47
  constant_a = -8;
  result = (constant_a == col_sep_length);
  uni_klee_add_patch(patch_results, 47, result);
  // Patch 7-3 # 48
  constant_a = -7;
  result = (constant_a == col_sep_length);
  uni_klee_add_patch(patch_results, 48, result);
  // Patch 7-4 # 49
  constant_a = -6;
  result = (constant_a == col_sep_length);
  uni_klee_add_patch(patch_results, 49, result);
  // Patch 7-5 # 50
  constant_a = -5;
  result = (constant_a == col_sep_length);
  uni_klee_add_patch(patch_results, 50, result);
  // Patch 7-6 # 51
  constant_a = -4;
  result = (constant_a == col_sep_length);
  uni_klee_add_patch(patch_results, 51, result);
  // Patch 7-7 # 52
  constant_a = -3;
  result = (constant_a == col_sep_length);
  uni_klee_add_patch(patch_results, 52, result);
  // Patch 7-8 # 53
  constant_a = -2;
  result = (constant_a == col_sep_length);
  uni_klee_add_patch(patch_results, 53, result);
  // Patch 7-9 # 54
  constant_a = -1;
  result = (constant_a == col_sep_length);
  uni_klee_add_patch(patch_results, 54, result);
  // Patch 7-10 # 55
  constant_a = 0;
  result = (constant_a == col_sep_length);
  uni_klee_add_patch(patch_results, 55, result);
  // Patch 7-11 # 56
  constant_a = 1;
  result = (constant_a == col_sep_length);
  uni_klee_add_patch(patch_results, 56, result);
  // Patch 7-12 # 57
  constant_a = 2;
  result = (constant_a == col_sep_length);
  uni_klee_add_patch(patch_results, 57, result);
  // Patch 7-13 # 58
  constant_a = 4;
  result = (constant_a == col_sep_length);
  uni_klee_add_patch(patch_results, 58, result);
  // Patch 7-14 # 59
  constant_a = 5;
  result = (constant_a == col_sep_length);
  uni_klee_add_patch(patch_results, 59, result);
  // Patch 7-15 # 60
  constant_a = 6;
  result = (constant_a == col_sep_length);
  uni_klee_add_patch(patch_results, 60, result);
  // Patch 7-16 # 61
  constant_a = 7;
  result = (constant_a == col_sep_length);
  uni_klee_add_patch(patch_results, 61, result);
  // Patch 7-17 # 62
  constant_a = 8;
  result = (constant_a == col_sep_length);
  uni_klee_add_patch(patch_results, 62, result);
  // Patch 7-18 # 63
  constant_a = 9;
  result = (constant_a == col_sep_length);
  uni_klee_add_patch(patch_results, 63, result);
  // Patch 7-19 # 64
  constant_a = 10;
  result = (constant_a == col_sep_length);
  uni_klee_add_patch(patch_results, 64, result);
  // Patch correct # 65
  result = (col_sep_length == 1);
  uni_klee_add_patch(patch_results, 65, result);
  klee_select_patch(&uni_klee_patch_id);
  return uni_klee_choice(patch_results, uni_klee_patch_id);
}
// UNI_KLEE_END

int __cpr_output(char* id, char* typestr, int value){
  return value;
}
