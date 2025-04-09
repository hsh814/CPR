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
  long long size = rvals[0];
  long long i = rvals[1];
  long long constant_a;
  int patch_results[933];
  // Patch buggy # 0
  result = (size / 2 > i);
  uni_klee_add_patch(patch_results, 0, result);
  // Patch 1-0 # 1
  constant_a = 0;
  result = ((constant_a / i) == constant_a);
  uni_klee_add_patch(patch_results, 1, result);
  // Patch 2-0 # 2
  constant_a = 0;
  result = ((constant_a / size) == constant_a);
  uni_klee_add_patch(patch_results, 2, result);
  // Patch 3-0 # 3
  result = (i == i);
  uni_klee_add_patch(patch_results, 3, result);
  // Patch 4-0 # 4
  result = (i < size);
  uni_klee_add_patch(patch_results, 4, result);
  // Patch 5-0 # 5
  result = (size != i);
  uni_klee_add_patch(patch_results, 5, result);
  // Patch 6-0 # 6
  result = (i <= size);
  uni_klee_add_patch(patch_results, 6, result);
  // Patch 7-0 # 7
  constant_a = 1;
  result = ((i / constant_a) == i);
  uni_klee_add_patch(patch_results, 7, result);
  // Patch 8-0 # 8
  constant_a = 1;
  result = (i <= (i / constant_a));
  uni_klee_add_patch(patch_results, 8, result);
  // Patch 9-0 # 9
  constant_a = 0;
  result = ((i / size) == constant_a);
  uni_klee_add_patch(patch_results, 9, result);
  // Patch 10-0 # 10
  constant_a = 1;
  result = (i < (size / constant_a));
  uni_klee_add_patch(patch_results, 10, result);
  // Patch 11-0 # 11
  constant_a = 10;
  result = ((size / i) <= constant_a);
  uni_klee_add_patch(patch_results, 11, result);
  // Patch 12-0 # 12
  constant_a = 1;
  result = ((size / constant_a) == size);
  uni_klee_add_patch(patch_results, 12, result);
  // Patch 13-0 # 13
  constant_a = 1;
  result = (size <= (size / constant_a));
  uni_klee_add_patch(patch_results, 13, result);
  // Patch 14-0 # 14
  result = ((i / size) < i);
  uni_klee_add_patch(patch_results, 14, result);
  // Patch 15-0 # 15
  result = ((size / i) != i);
  uni_klee_add_patch(patch_results, 15, result);
  // Patch 16-0 # 16
  result = ((i / size) != i);
  uni_klee_add_patch(patch_results, 16, result);
  // Patch 17-0 # 17
  result = ((i / size) <= i);
  uni_klee_add_patch(patch_results, 17, result);
  // Patch 18-0 # 18
  result = ((i / size) < size);
  uni_klee_add_patch(patch_results, 18, result);
  // Patch 19-0 # 19
  result = ((size / i) < size);
  uni_klee_add_patch(patch_results, 19, result);
  // Patch 20-0 # 20
  result = ((i / size) != size);
  uni_klee_add_patch(patch_results, 20, result);
  // Patch 21-0 # 21
  result = ((size / i) != size);
  uni_klee_add_patch(patch_results, 21, result);
  // Patch 22-0 # 22
  result = ((i / size) <= size);
  uni_klee_add_patch(patch_results, 22, result);
  // Patch 23-0 # 23
  result = ((size / i) <= size);
  uni_klee_add_patch(patch_results, 23, result);
  // Patch 24-0 # 24
  result = ((size / i) != (i / i));
  uni_klee_add_patch(patch_results, 24, result);
  // Patch 25-0 # 25
  constant_a = 1;
  result = (i <= (size / constant_a));
  uni_klee_add_patch(patch_results, 25, result);
  // Patch 25-1 # 26
  constant_a = 2;
  result = (i <= (size / constant_a));
  uni_klee_add_patch(patch_results, 26, result);
  // Patch 26-0 # 27
  constant_a = 7;
  result = (i < constant_a);
  uni_klee_add_patch(patch_results, 27, result);
  // Patch 26-1 # 28
  constant_a = 8;
  result = (i < constant_a);
  uni_klee_add_patch(patch_results, 28, result);
  // Patch 26-2 # 29
  constant_a = 9;
  result = (i < constant_a);
  uni_klee_add_patch(patch_results, 29, result);
  // Patch 26-3 # 30
  constant_a = 10;
  result = (i < constant_a);
  uni_klee_add_patch(patch_results, 30, result);
  // Patch 27-0 # 31
  constant_a = 6;
  result = (i <= constant_a);
  uni_klee_add_patch(patch_results, 31, result);
  // Patch 27-1 # 32
  constant_a = 7;
  result = (i <= constant_a);
  uni_klee_add_patch(patch_results, 32, result);
  // Patch 27-2 # 33
  constant_a = 8;
  result = (i <= constant_a);
  uni_klee_add_patch(patch_results, 33, result);
  // Patch 27-3 # 34
  constant_a = 9;
  result = (i <= constant_a);
  uni_klee_add_patch(patch_results, 34, result);
  // Patch 27-4 # 35
  constant_a = 10;
  result = (i <= constant_a);
  uni_klee_add_patch(patch_results, 35, result);
  // Patch 28-0 # 36
  constant_a = -10;
  result = (constant_a < (size / constant_a));
  uni_klee_add_patch(patch_results, 36, result);
  // Patch 28-1 # 37
  constant_a = -9;
  result = (constant_a < (size / constant_a));
  uni_klee_add_patch(patch_results, 37, result);
  // Patch 28-2 # 38
  constant_a = -8;
  result = (constant_a < (size / constant_a));
  uni_klee_add_patch(patch_results, 38, result);
  // Patch 28-3 # 39
  constant_a = -7;
  result = (constant_a < (size / constant_a));
  uni_klee_add_patch(patch_results, 39, result);
  // Patch 28-4 # 40
  constant_a = -6;
  result = (constant_a < (size / constant_a));
  uni_klee_add_patch(patch_results, 40, result);
  // Patch 28-5 # 41
  constant_a = 1;
  result = (constant_a < (size / constant_a));
  uni_klee_add_patch(patch_results, 41, result);
  // Patch 28-6 # 42
  constant_a = 2;
  result = (constant_a < (size / constant_a));
  uni_klee_add_patch(patch_results, 42, result);
  // Patch 29-0 # 43
  constant_a = -10;
  result = (constant_a <= (size / constant_a));
  uni_klee_add_patch(patch_results, 43, result);
  // Patch 29-1 # 44
  constant_a = -9;
  result = (constant_a <= (size / constant_a));
  uni_klee_add_patch(patch_results, 44, result);
  // Patch 29-2 # 45
  constant_a = -8;
  result = (constant_a <= (size / constant_a));
  uni_klee_add_patch(patch_results, 45, result);
  // Patch 29-3 # 46
  constant_a = -7;
  result = (constant_a <= (size / constant_a));
  uni_klee_add_patch(patch_results, 46, result);
  // Patch 29-4 # 47
  constant_a = -6;
  result = (constant_a <= (size / constant_a));
  uni_klee_add_patch(patch_results, 47, result);
  // Patch 29-5 # 48
  constant_a = 1;
  result = (constant_a <= (size / constant_a));
  uni_klee_add_patch(patch_results, 48, result);
  // Patch 29-6 # 49
  constant_a = 2;
  result = (constant_a <= (size / constant_a));
  uni_klee_add_patch(patch_results, 49, result);
  // Patch 30-0 # 50
  constant_a = -2;
  result = ((size / constant_a) < constant_a);
  uni_klee_add_patch(patch_results, 50, result);
  // Patch 30-1 # 51
  constant_a = -1;
  result = ((size / constant_a) < constant_a);
  uni_klee_add_patch(patch_results, 51, result);
  // Patch 30-2 # 52
  constant_a = 6;
  result = ((size / constant_a) < constant_a);
  uni_klee_add_patch(patch_results, 52, result);
  // Patch 30-3 # 53
  constant_a = 7;
  result = ((size / constant_a) < constant_a);
  uni_klee_add_patch(patch_results, 53, result);
  // Patch 30-4 # 54
  constant_a = 8;
  result = ((size / constant_a) < constant_a);
  uni_klee_add_patch(patch_results, 54, result);
  // Patch 30-5 # 55
  constant_a = 9;
  result = ((size / constant_a) < constant_a);
  uni_klee_add_patch(patch_results, 55, result);
  // Patch 30-6 # 56
  constant_a = 10;
  result = ((size / constant_a) < constant_a);
  uni_klee_add_patch(patch_results, 56, result);
  // Patch 31-0 # 57
  constant_a = -2;
  result = ((size / constant_a) <= constant_a);
  uni_klee_add_patch(patch_results, 57, result);
  // Patch 31-1 # 58
  constant_a = -1;
  result = ((size / constant_a) <= constant_a);
  uni_klee_add_patch(patch_results, 58, result);
  // Patch 31-2 # 59
  constant_a = 6;
  result = ((size / constant_a) <= constant_a);
  uni_klee_add_patch(patch_results, 59, result);
  // Patch 31-3 # 60
  constant_a = 7;
  result = ((size / constant_a) <= constant_a);
  uni_klee_add_patch(patch_results, 60, result);
  // Patch 31-4 # 61
  constant_a = 8;
  result = ((size / constant_a) <= constant_a);
  uni_klee_add_patch(patch_results, 61, result);
  // Patch 31-5 # 62
  constant_a = 9;
  result = ((size / constant_a) <= constant_a);
  uni_klee_add_patch(patch_results, 62, result);
  // Patch 31-6 # 63
  constant_a = 10;
  result = ((size / constant_a) <= constant_a);
  uni_klee_add_patch(patch_results, 63, result);
  // Patch 32-0 # 64
  constant_a = -10;
  result = (constant_a < (i / constant_a));
  uni_klee_add_patch(patch_results, 64, result);
  // Patch 32-1 # 65
  constant_a = -9;
  result = (constant_a < (i / constant_a));
  uni_klee_add_patch(patch_results, 65, result);
  // Patch 32-2 # 66
  constant_a = -8;
  result = (constant_a < (i / constant_a));
  uni_klee_add_patch(patch_results, 66, result);
  // Patch 32-3 # 67
  constant_a = -7;
  result = (constant_a < (i / constant_a));
  uni_klee_add_patch(patch_results, 67, result);
  // Patch 32-4 # 68
  constant_a = -6;
  result = (constant_a < (i / constant_a));
  uni_klee_add_patch(patch_results, 68, result);
  // Patch 32-5 # 69
  constant_a = -5;
  result = (constant_a < (i / constant_a));
  uni_klee_add_patch(patch_results, 69, result);
  // Patch 32-6 # 70
  constant_a = -4;
  result = (constant_a < (i / constant_a));
  uni_klee_add_patch(patch_results, 70, result);
  // Patch 32-7 # 71
  constant_a = -3;
  result = (constant_a < (i / constant_a));
  uni_klee_add_patch(patch_results, 71, result);
  // Patch 32-8 # 72
  constant_a = 1;
  result = (constant_a < (i / constant_a));
  uni_klee_add_patch(patch_results, 72, result);
  // Patch 33-0 # 73
  constant_a = -10;
  result = (constant_a <= (i / constant_a));
  uni_klee_add_patch(patch_results, 73, result);
  // Patch 33-1 # 74
  constant_a = -9;
  result = (constant_a <= (i / constant_a));
  uni_klee_add_patch(patch_results, 74, result);
  // Patch 33-2 # 75
  constant_a = -8;
  result = (constant_a <= (i / constant_a));
  uni_klee_add_patch(patch_results, 75, result);
  // Patch 33-3 # 76
  constant_a = -7;
  result = (constant_a <= (i / constant_a));
  uni_klee_add_patch(patch_results, 76, result);
  // Patch 33-4 # 77
  constant_a = -6;
  result = (constant_a <= (i / constant_a));
  uni_klee_add_patch(patch_results, 77, result);
  // Patch 33-5 # 78
  constant_a = -5;
  result = (constant_a <= (i / constant_a));
  uni_klee_add_patch(patch_results, 78, result);
  // Patch 33-6 # 79
  constant_a = -4;
  result = (constant_a <= (i / constant_a));
  uni_klee_add_patch(patch_results, 79, result);
  // Patch 33-7 # 80
  constant_a = -3;
  result = (constant_a <= (i / constant_a));
  uni_klee_add_patch(patch_results, 80, result);
  // Patch 33-8 # 81
  constant_a = 1;
  result = (constant_a <= (i / constant_a));
  uni_klee_add_patch(patch_results, 81, result);
  // Patch 34-0 # 82
  constant_a = -1;
  result = ((i / constant_a) < constant_a);
  uni_klee_add_patch(patch_results, 82, result);
  // Patch 34-1 # 83
  constant_a = 3;
  result = ((i / constant_a) < constant_a);
  uni_klee_add_patch(patch_results, 83, result);
  // Patch 34-2 # 84
  constant_a = 4;
  result = ((i / constant_a) < constant_a);
  uni_klee_add_patch(patch_results, 84, result);
  // Patch 34-3 # 85
  constant_a = 5;
  result = ((i / constant_a) < constant_a);
  uni_klee_add_patch(patch_results, 85, result);
  // Patch 34-4 # 86
  constant_a = 6;
  result = ((i / constant_a) < constant_a);
  uni_klee_add_patch(patch_results, 86, result);
  // Patch 34-5 # 87
  constant_a = 7;
  result = ((i / constant_a) < constant_a);
  uni_klee_add_patch(patch_results, 87, result);
  // Patch 34-6 # 88
  constant_a = 8;
  result = ((i / constant_a) < constant_a);
  uni_klee_add_patch(patch_results, 88, result);
  // Patch 34-7 # 89
  constant_a = 9;
  result = ((i / constant_a) < constant_a);
  uni_klee_add_patch(patch_results, 89, result);
  // Patch 34-8 # 90
  constant_a = 10;
  result = ((i / constant_a) < constant_a);
  uni_klee_add_patch(patch_results, 90, result);
  // Patch 35-0 # 91
  constant_a = -10;
  result = (constant_a < (constant_a / i));
  uni_klee_add_patch(patch_results, 91, result);
  // Patch 35-1 # 92
  constant_a = -9;
  result = (constant_a < (constant_a / i));
  uni_klee_add_patch(patch_results, 92, result);
  // Patch 35-2 # 93
  constant_a = -8;
  result = (constant_a < (constant_a / i));
  uni_klee_add_patch(patch_results, 93, result);
  // Patch 35-3 # 94
  constant_a = -7;
  result = (constant_a < (constant_a / i));
  uni_klee_add_patch(patch_results, 94, result);
  // Patch 35-4 # 95
  constant_a = -6;
  result = (constant_a < (constant_a / i));
  uni_klee_add_patch(patch_results, 95, result);
  // Patch 35-5 # 96
  constant_a = -5;
  result = (constant_a < (constant_a / i));
  uni_klee_add_patch(patch_results, 96, result);
  // Patch 35-6 # 97
  constant_a = -4;
  result = (constant_a < (constant_a / i));
  uni_klee_add_patch(patch_results, 97, result);
  // Patch 35-7 # 98
  constant_a = -3;
  result = (constant_a < (constant_a / i));
  uni_klee_add_patch(patch_results, 98, result);
  // Patch 35-8 # 99
  constant_a = -2;
  result = (constant_a < (constant_a / i));
  uni_klee_add_patch(patch_results, 99, result);
  // Patch 35-9 # 100
  constant_a = -1;
  result = (constant_a < (constant_a / i));
  uni_klee_add_patch(patch_results, 100, result);
  // Patch 36-0 # 101
  constant_a = 1;
  result = ((constant_a / i) < constant_a);
  uni_klee_add_patch(patch_results, 101, result);
  // Patch 36-1 # 102
  constant_a = 2;
  result = ((constant_a / i) < constant_a);
  uni_klee_add_patch(patch_results, 102, result);
  // Patch 36-2 # 103
  constant_a = 3;
  result = ((constant_a / i) < constant_a);
  uni_klee_add_patch(patch_results, 103, result);
  // Patch 36-3 # 104
  constant_a = 4;
  result = ((constant_a / i) < constant_a);
  uni_klee_add_patch(patch_results, 104, result);
  // Patch 36-4 # 105
  constant_a = 5;
  result = ((constant_a / i) < constant_a);
  uni_klee_add_patch(patch_results, 105, result);
  // Patch 36-5 # 106
  constant_a = 6;
  result = ((constant_a / i) < constant_a);
  uni_klee_add_patch(patch_results, 106, result);
  // Patch 36-6 # 107
  constant_a = 7;
  result = ((constant_a / i) < constant_a);
  uni_klee_add_patch(patch_results, 107, result);
  // Patch 36-7 # 108
  constant_a = 8;
  result = ((constant_a / i) < constant_a);
  uni_klee_add_patch(patch_results, 108, result);
  // Patch 36-8 # 109
  constant_a = 9;
  result = ((constant_a / i) < constant_a);
  uni_klee_add_patch(patch_results, 109, result);
  // Patch 36-9 # 110
  constant_a = 10;
  result = ((constant_a / i) < constant_a);
  uni_klee_add_patch(patch_results, 110, result);
  // Patch 37-0 # 111
  constant_a = -1;
  result = ((i / constant_a) <= constant_a);
  uni_klee_add_patch(patch_results, 111, result);
  // Patch 37-1 # 112
  constant_a = 3;
  result = ((i / constant_a) <= constant_a);
  uni_klee_add_patch(patch_results, 112, result);
  // Patch 37-2 # 113
  constant_a = 4;
  result = ((i / constant_a) <= constant_a);
  uni_klee_add_patch(patch_results, 113, result);
  // Patch 37-3 # 114
  constant_a = 5;
  result = ((i / constant_a) <= constant_a);
  uni_klee_add_patch(patch_results, 114, result);
  // Patch 37-4 # 115
  constant_a = 6;
  result = ((i / constant_a) <= constant_a);
  uni_klee_add_patch(patch_results, 115, result);
  // Patch 37-5 # 116
  constant_a = 7;
  result = ((i / constant_a) <= constant_a);
  uni_klee_add_patch(patch_results, 116, result);
  // Patch 37-6 # 117
  constant_a = 8;
  result = ((i / constant_a) <= constant_a);
  uni_klee_add_patch(patch_results, 117, result);
  // Patch 37-7 # 118
  constant_a = 9;
  result = ((i / constant_a) <= constant_a);
  uni_klee_add_patch(patch_results, 118, result);
  // Patch 37-8 # 119
  constant_a = 10;
  result = ((i / constant_a) <= constant_a);
  uni_klee_add_patch(patch_results, 119, result);
  // Patch 38-0 # 120
  constant_a = 1;
  result = ((constant_a / size) < constant_a);
  uni_klee_add_patch(patch_results, 120, result);
  // Patch 38-1 # 121
  constant_a = 2;
  result = ((constant_a / size) < constant_a);
  uni_klee_add_patch(patch_results, 121, result);
  // Patch 38-2 # 122
  constant_a = 3;
  result = ((constant_a / size) < constant_a);
  uni_klee_add_patch(patch_results, 122, result);
  // Patch 38-3 # 123
  constant_a = 4;
  result = ((constant_a / size) < constant_a);
  uni_klee_add_patch(patch_results, 123, result);
  // Patch 38-4 # 124
  constant_a = 5;
  result = ((constant_a / size) < constant_a);
  uni_klee_add_patch(patch_results, 124, result);
  // Patch 38-5 # 125
  constant_a = 6;
  result = ((constant_a / size) < constant_a);
  uni_klee_add_patch(patch_results, 125, result);
  // Patch 38-6 # 126
  constant_a = 7;
  result = ((constant_a / size) < constant_a);
  uni_klee_add_patch(patch_results, 126, result);
  // Patch 38-7 # 127
  constant_a = 8;
  result = ((constant_a / size) < constant_a);
  uni_klee_add_patch(patch_results, 127, result);
  // Patch 38-8 # 128
  constant_a = 9;
  result = ((constant_a / size) < constant_a);
  uni_klee_add_patch(patch_results, 128, result);
  // Patch 38-9 # 129
  constant_a = 10;
  result = ((constant_a / size) < constant_a);
  uni_klee_add_patch(patch_results, 129, result);
  // Patch 39-0 # 130
  constant_a = -10;
  result = (constant_a < (constant_a / size));
  uni_klee_add_patch(patch_results, 130, result);
  // Patch 39-1 # 131
  constant_a = -9;
  result = (constant_a < (constant_a / size));
  uni_klee_add_patch(patch_results, 131, result);
  // Patch 39-2 # 132
  constant_a = -8;
  result = (constant_a < (constant_a / size));
  uni_klee_add_patch(patch_results, 132, result);
  // Patch 39-3 # 133
  constant_a = -7;
  result = (constant_a < (constant_a / size));
  uni_klee_add_patch(patch_results, 133, result);
  // Patch 39-4 # 134
  constant_a = -6;
  result = (constant_a < (constant_a / size));
  uni_klee_add_patch(patch_results, 134, result);
  // Patch 39-5 # 135
  constant_a = -5;
  result = (constant_a < (constant_a / size));
  uni_klee_add_patch(patch_results, 135, result);
  // Patch 39-6 # 136
  constant_a = -4;
  result = (constant_a < (constant_a / size));
  uni_klee_add_patch(patch_results, 136, result);
  // Patch 39-7 # 137
  constant_a = -3;
  result = (constant_a < (constant_a / size));
  uni_klee_add_patch(patch_results, 137, result);
  // Patch 39-8 # 138
  constant_a = -2;
  result = (constant_a < (constant_a / size));
  uni_klee_add_patch(patch_results, 138, result);
  // Patch 39-9 # 139
  constant_a = -1;
  result = (constant_a < (constant_a / size));
  uni_klee_add_patch(patch_results, 139, result);
  // Patch 40-0 # 140
  constant_a = -10;
  result = (constant_a < (i / size));
  uni_klee_add_patch(patch_results, 140, result);
  // Patch 40-1 # 141
  constant_a = -9;
  result = (constant_a < (i / size));
  uni_klee_add_patch(patch_results, 141, result);
  // Patch 40-2 # 142
  constant_a = -8;
  result = (constant_a < (i / size));
  uni_klee_add_patch(patch_results, 142, result);
  // Patch 40-3 # 143
  constant_a = -7;
  result = (constant_a < (i / size));
  uni_klee_add_patch(patch_results, 143, result);
  // Patch 40-4 # 144
  constant_a = -6;
  result = (constant_a < (i / size));
  uni_klee_add_patch(patch_results, 144, result);
  // Patch 40-5 # 145
  constant_a = -5;
  result = (constant_a < (i / size));
  uni_klee_add_patch(patch_results, 145, result);
  // Patch 40-6 # 146
  constant_a = -4;
  result = (constant_a < (i / size));
  uni_klee_add_patch(patch_results, 146, result);
  // Patch 40-7 # 147
  constant_a = -3;
  result = (constant_a < (i / size));
  uni_klee_add_patch(patch_results, 147, result);
  // Patch 40-8 # 148
  constant_a = -2;
  result = (constant_a < (i / size));
  uni_klee_add_patch(patch_results, 148, result);
  // Patch 40-9 # 149
  constant_a = -1;
  result = (constant_a < (i / size));
  uni_klee_add_patch(patch_results, 149, result);
  // Patch 41-0 # 150
  constant_a = 1;
  result = ((i / size) < constant_a);
  uni_klee_add_patch(patch_results, 150, result);
  // Patch 41-1 # 151
  constant_a = 2;
  result = ((i / size) < constant_a);
  uni_klee_add_patch(patch_results, 151, result);
  // Patch 41-2 # 152
  constant_a = 3;
  result = ((i / size) < constant_a);
  uni_klee_add_patch(patch_results, 152, result);
  // Patch 41-3 # 153
  constant_a = 4;
  result = ((i / size) < constant_a);
  uni_klee_add_patch(patch_results, 153, result);
  // Patch 41-4 # 154
  constant_a = 5;
  result = ((i / size) < constant_a);
  uni_klee_add_patch(patch_results, 154, result);
  // Patch 41-5 # 155
  constant_a = 6;
  result = ((i / size) < constant_a);
  uni_klee_add_patch(patch_results, 155, result);
  // Patch 41-6 # 156
  constant_a = 7;
  result = ((i / size) < constant_a);
  uni_klee_add_patch(patch_results, 156, result);
  // Patch 41-7 # 157
  constant_a = 8;
  result = ((i / size) < constant_a);
  uni_klee_add_patch(patch_results, 157, result);
  // Patch 41-8 # 158
  constant_a = 9;
  result = ((i / size) < constant_a);
  uni_klee_add_patch(patch_results, 158, result);
  // Patch 41-9 # 159
  constant_a = 10;
  result = ((i / size) < constant_a);
  uni_klee_add_patch(patch_results, 159, result);
  // Patch 42-0 # 160
  constant_a = -10;
  result = (constant_a <= (constant_a / i));
  uni_klee_add_patch(patch_results, 160, result);
  // Patch 42-1 # 161
  constant_a = -9;
  result = (constant_a <= (constant_a / i));
  uni_klee_add_patch(patch_results, 161, result);
  // Patch 42-2 # 162
  constant_a = -8;
  result = (constant_a <= (constant_a / i));
  uni_klee_add_patch(patch_results, 162, result);
  // Patch 42-3 # 163
  constant_a = -7;
  result = (constant_a <= (constant_a / i));
  uni_klee_add_patch(patch_results, 163, result);
  // Patch 42-4 # 164
  constant_a = -6;
  result = (constant_a <= (constant_a / i));
  uni_klee_add_patch(patch_results, 164, result);
  // Patch 42-5 # 165
  constant_a = -5;
  result = (constant_a <= (constant_a / i));
  uni_klee_add_patch(patch_results, 165, result);
  // Patch 42-6 # 166
  constant_a = -4;
  result = (constant_a <= (constant_a / i));
  uni_klee_add_patch(patch_results, 166, result);
  // Patch 42-7 # 167
  constant_a = -3;
  result = (constant_a <= (constant_a / i));
  uni_klee_add_patch(patch_results, 167, result);
  // Patch 42-8 # 168
  constant_a = -2;
  result = (constant_a <= (constant_a / i));
  uni_klee_add_patch(patch_results, 168, result);
  // Patch 42-9 # 169
  constant_a = -1;
  result = (constant_a <= (constant_a / i));
  uni_klee_add_patch(patch_results, 169, result);
  // Patch 42-10 # 170
  constant_a = 0;
  result = (constant_a <= (constant_a / i));
  uni_klee_add_patch(patch_results, 170, result);
  // Patch 43-0 # 171
  constant_a = 0;
  result = ((constant_a / i) <= constant_a);
  uni_klee_add_patch(patch_results, 171, result);
  // Patch 43-1 # 172
  constant_a = 1;
  result = ((constant_a / i) <= constant_a);
  uni_klee_add_patch(patch_results, 172, result);
  // Patch 43-2 # 173
  constant_a = 2;
  result = ((constant_a / i) <= constant_a);
  uni_klee_add_patch(patch_results, 173, result);
  // Patch 43-3 # 174
  constant_a = 3;
  result = ((constant_a / i) <= constant_a);
  uni_klee_add_patch(patch_results, 174, result);
  // Patch 43-4 # 175
  constant_a = 4;
  result = ((constant_a / i) <= constant_a);
  uni_klee_add_patch(patch_results, 175, result);
  // Patch 43-5 # 176
  constant_a = 5;
  result = ((constant_a / i) <= constant_a);
  uni_klee_add_patch(patch_results, 176, result);
  // Patch 43-6 # 177
  constant_a = 6;
  result = ((constant_a / i) <= constant_a);
  uni_klee_add_patch(patch_results, 177, result);
  // Patch 43-7 # 178
  constant_a = 7;
  result = ((constant_a / i) <= constant_a);
  uni_klee_add_patch(patch_results, 178, result);
  // Patch 43-8 # 179
  constant_a = 8;
  result = ((constant_a / i) <= constant_a);
  uni_klee_add_patch(patch_results, 179, result);
  // Patch 43-9 # 180
  constant_a = 9;
  result = ((constant_a / i) <= constant_a);
  uni_klee_add_patch(patch_results, 180, result);
  // Patch 43-10 # 181
  constant_a = 10;
  result = ((constant_a / i) <= constant_a);
  uni_klee_add_patch(patch_results, 181, result);
  // Patch 44-0 # 182
  constant_a = -10;
  result = (constant_a <= (constant_a / size));
  uni_klee_add_patch(patch_results, 182, result);
  // Patch 44-1 # 183
  constant_a = -9;
  result = (constant_a <= (constant_a / size));
  uni_klee_add_patch(patch_results, 183, result);
  // Patch 44-2 # 184
  constant_a = -8;
  result = (constant_a <= (constant_a / size));
  uni_klee_add_patch(patch_results, 184, result);
  // Patch 44-3 # 185
  constant_a = -7;
  result = (constant_a <= (constant_a / size));
  uni_klee_add_patch(patch_results, 185, result);
  // Patch 44-4 # 186
  constant_a = -6;
  result = (constant_a <= (constant_a / size));
  uni_klee_add_patch(patch_results, 186, result);
  // Patch 44-5 # 187
  constant_a = -5;
  result = (constant_a <= (constant_a / size));
  uni_klee_add_patch(patch_results, 187, result);
  // Patch 44-6 # 188
  constant_a = -4;
  result = (constant_a <= (constant_a / size));
  uni_klee_add_patch(patch_results, 188, result);
  // Patch 44-7 # 189
  constant_a = -3;
  result = (constant_a <= (constant_a / size));
  uni_klee_add_patch(patch_results, 189, result);
  // Patch 44-8 # 190
  constant_a = -2;
  result = (constant_a <= (constant_a / size));
  uni_klee_add_patch(patch_results, 190, result);
  // Patch 44-9 # 191
  constant_a = -1;
  result = (constant_a <= (constant_a / size));
  uni_klee_add_patch(patch_results, 191, result);
  // Patch 44-10 # 192
  constant_a = 0;
  result = (constant_a <= (constant_a / size));
  uni_klee_add_patch(patch_results, 192, result);
  // Patch 45-0 # 193
  constant_a = 0;
  result = ((constant_a / size) <= constant_a);
  uni_klee_add_patch(patch_results, 193, result);
  // Patch 45-1 # 194
  constant_a = 1;
  result = ((constant_a / size) <= constant_a);
  uni_klee_add_patch(patch_results, 194, result);
  // Patch 45-2 # 195
  constant_a = 2;
  result = ((constant_a / size) <= constant_a);
  uni_klee_add_patch(patch_results, 195, result);
  // Patch 45-3 # 196
  constant_a = 3;
  result = ((constant_a / size) <= constant_a);
  uni_klee_add_patch(patch_results, 196, result);
  // Patch 45-4 # 197
  constant_a = 4;
  result = ((constant_a / size) <= constant_a);
  uni_klee_add_patch(patch_results, 197, result);
  // Patch 45-5 # 198
  constant_a = 5;
  result = ((constant_a / size) <= constant_a);
  uni_klee_add_patch(patch_results, 198, result);
  // Patch 45-6 # 199
  constant_a = 6;
  result = ((constant_a / size) <= constant_a);
  uni_klee_add_patch(patch_results, 199, result);
  // Patch 45-7 # 200
  constant_a = 7;
  result = ((constant_a / size) <= constant_a);
  uni_klee_add_patch(patch_results, 200, result);
  // Patch 45-8 # 201
  constant_a = 8;
  result = ((constant_a / size) <= constant_a);
  uni_klee_add_patch(patch_results, 201, result);
  // Patch 45-9 # 202
  constant_a = 9;
  result = ((constant_a / size) <= constant_a);
  uni_klee_add_patch(patch_results, 202, result);
  // Patch 45-10 # 203
  constant_a = 10;
  result = ((constant_a / size) <= constant_a);
  uni_klee_add_patch(patch_results, 203, result);
  // Patch 46-0 # 204
  constant_a = -10;
  result = ((size / constant_a) < i);
  uni_klee_add_patch(patch_results, 204, result);
  // Patch 46-1 # 205
  constant_a = -9;
  result = ((size / constant_a) < i);
  uni_klee_add_patch(patch_results, 205, result);
  // Patch 46-2 # 206
  constant_a = -8;
  result = ((size / constant_a) < i);
  uni_klee_add_patch(patch_results, 206, result);
  // Patch 46-3 # 207
  constant_a = -7;
  result = ((size / constant_a) < i);
  uni_klee_add_patch(patch_results, 207, result);
  // Patch 46-4 # 208
  constant_a = -6;
  result = ((size / constant_a) < i);
  uni_klee_add_patch(patch_results, 208, result);
  // Patch 46-5 # 209
  constant_a = -5;
  result = ((size / constant_a) < i);
  uni_klee_add_patch(patch_results, 209, result);
  // Patch 46-6 # 210
  constant_a = -4;
  result = ((size / constant_a) < i);
  uni_klee_add_patch(patch_results, 210, result);
  // Patch 46-7 # 211
  constant_a = -3;
  result = ((size / constant_a) < i);
  uni_klee_add_patch(patch_results, 211, result);
  // Patch 46-8 # 212
  constant_a = -2;
  result = ((size / constant_a) < i);
  uni_klee_add_patch(patch_results, 212, result);
  // Patch 46-9 # 213
  constant_a = -1;
  result = ((size / constant_a) < i);
  uni_klee_add_patch(patch_results, 213, result);
  // Patch 47-0 # 214
  constant_a = 0;
  result = ((i / size) <= constant_a);
  uni_klee_add_patch(patch_results, 214, result);
  // Patch 47-1 # 215
  constant_a = 1;
  result = ((i / size) <= constant_a);
  uni_klee_add_patch(patch_results, 215, result);
  // Patch 47-2 # 216
  constant_a = 2;
  result = ((i / size) <= constant_a);
  uni_klee_add_patch(patch_results, 216, result);
  // Patch 47-3 # 217
  constant_a = 3;
  result = ((i / size) <= constant_a);
  uni_klee_add_patch(patch_results, 217, result);
  // Patch 47-4 # 218
  constant_a = 4;
  result = ((i / size) <= constant_a);
  uni_klee_add_patch(patch_results, 218, result);
  // Patch 47-5 # 219
  constant_a = 5;
  result = ((i / size) <= constant_a);
  uni_klee_add_patch(patch_results, 219, result);
  // Patch 47-6 # 220
  constant_a = 6;
  result = ((i / size) <= constant_a);
  uni_klee_add_patch(patch_results, 220, result);
  // Patch 47-7 # 221
  constant_a = 7;
  result = ((i / size) <= constant_a);
  uni_klee_add_patch(patch_results, 221, result);
  // Patch 47-8 # 222
  constant_a = 8;
  result = ((i / size) <= constant_a);
  uni_klee_add_patch(patch_results, 222, result);
  // Patch 47-9 # 223
  constant_a = 9;
  result = ((i / size) <= constant_a);
  uni_klee_add_patch(patch_results, 223, result);
  // Patch 47-10 # 224
  constant_a = 10;
  result = ((i / size) <= constant_a);
  uni_klee_add_patch(patch_results, 224, result);
  // Patch 48-0 # 225
  constant_a = -10;
  result = (constant_a <= (i / size));
  uni_klee_add_patch(patch_results, 225, result);
  // Patch 48-1 # 226
  constant_a = -9;
  result = (constant_a <= (i / size));
  uni_klee_add_patch(patch_results, 226, result);
  // Patch 48-2 # 227
  constant_a = -8;
  result = (constant_a <= (i / size));
  uni_klee_add_patch(patch_results, 227, result);
  // Patch 48-3 # 228
  constant_a = -7;
  result = (constant_a <= (i / size));
  uni_klee_add_patch(patch_results, 228, result);
  // Patch 48-4 # 229
  constant_a = -6;
  result = (constant_a <= (i / size));
  uni_klee_add_patch(patch_results, 229, result);
  // Patch 48-5 # 230
  constant_a = -5;
  result = (constant_a <= (i / size));
  uni_klee_add_patch(patch_results, 230, result);
  // Patch 48-6 # 231
  constant_a = -4;
  result = (constant_a <= (i / size));
  uni_klee_add_patch(patch_results, 231, result);
  // Patch 48-7 # 232
  constant_a = -3;
  result = (constant_a <= (i / size));
  uni_klee_add_patch(patch_results, 232, result);
  // Patch 48-8 # 233
  constant_a = -2;
  result = (constant_a <= (i / size));
  uni_klee_add_patch(patch_results, 233, result);
  // Patch 48-9 # 234
  constant_a = -1;
  result = (constant_a <= (i / size));
  uni_klee_add_patch(patch_results, 234, result);
  // Patch 48-10 # 235
  constant_a = 0;
  result = (constant_a <= (i / size));
  uni_klee_add_patch(patch_results, 235, result);
  // Patch 49-0 # 236
  constant_a = -10;
  result = (constant_a < (size / i));
  uni_klee_add_patch(patch_results, 236, result);
  // Patch 49-1 # 237
  constant_a = -9;
  result = (constant_a < (size / i));
  uni_klee_add_patch(patch_results, 237, result);
  // Patch 49-2 # 238
  constant_a = -8;
  result = (constant_a < (size / i));
  uni_klee_add_patch(patch_results, 238, result);
  // Patch 49-3 # 239
  constant_a = -7;
  result = (constant_a < (size / i));
  uni_klee_add_patch(patch_results, 239, result);
  // Patch 49-4 # 240
  constant_a = -6;
  result = (constant_a < (size / i));
  uni_klee_add_patch(patch_results, 240, result);
  // Patch 49-5 # 241
  constant_a = -5;
  result = (constant_a < (size / i));
  uni_klee_add_patch(patch_results, 241, result);
  // Patch 49-6 # 242
  constant_a = -4;
  result = (constant_a < (size / i));
  uni_klee_add_patch(patch_results, 242, result);
  // Patch 49-7 # 243
  constant_a = -3;
  result = (constant_a < (size / i));
  uni_klee_add_patch(patch_results, 243, result);
  // Patch 49-8 # 244
  constant_a = -2;
  result = (constant_a < (size / i));
  uni_klee_add_patch(patch_results, 244, result);
  // Patch 49-9 # 245
  constant_a = -1;
  result = (constant_a < (size / i));
  uni_klee_add_patch(patch_results, 245, result);
  // Patch 49-10 # 246
  constant_a = 0;
  result = (constant_a < (size / i));
  uni_klee_add_patch(patch_results, 246, result);
  // Patch 49-11 # 247
  constant_a = 1;
  result = (constant_a < (size / i));
  uni_klee_add_patch(patch_results, 247, result);
  // Patch 50-0 # 248
  constant_a = -10;
  result = (constant_a < i);
  uni_klee_add_patch(patch_results, 248, result);
  // Patch 50-1 # 249
  constant_a = -9;
  result = (constant_a < i);
  uni_klee_add_patch(patch_results, 249, result);
  // Patch 50-2 # 250
  constant_a = -8;
  result = (constant_a < i);
  uni_klee_add_patch(patch_results, 250, result);
  // Patch 50-3 # 251
  constant_a = -7;
  result = (constant_a < i);
  uni_klee_add_patch(patch_results, 251, result);
  // Patch 50-4 # 252
  constant_a = -6;
  result = (constant_a < i);
  uni_klee_add_patch(patch_results, 252, result);
  // Patch 50-5 # 253
  constant_a = -5;
  result = (constant_a < i);
  uni_klee_add_patch(patch_results, 253, result);
  // Patch 50-6 # 254
  constant_a = -4;
  result = (constant_a < i);
  uni_klee_add_patch(patch_results, 254, result);
  // Patch 50-7 # 255
  constant_a = -3;
  result = (constant_a < i);
  uni_klee_add_patch(patch_results, 255, result);
  // Patch 50-8 # 256
  constant_a = -2;
  result = (constant_a < i);
  uni_klee_add_patch(patch_results, 256, result);
  // Patch 50-9 # 257
  constant_a = -1;
  result = (constant_a < i);
  uni_klee_add_patch(patch_results, 257, result);
  // Patch 50-10 # 258
  constant_a = 0;
  result = (constant_a < i);
  uni_klee_add_patch(patch_results, 258, result);
  // Patch 50-11 # 259
  constant_a = 1;
  result = (constant_a < i);
  uni_klee_add_patch(patch_results, 259, result);
  // Patch 50-12 # 260
  constant_a = 2;
  result = (constant_a < i);
  uni_klee_add_patch(patch_results, 260, result);
  // Patch 51-0 # 261
  constant_a = -10;
  result = ((size / constant_a) <= i);
  uni_klee_add_patch(patch_results, 261, result);
  // Patch 51-1 # 262
  constant_a = -9;
  result = ((size / constant_a) <= i);
  uni_klee_add_patch(patch_results, 262, result);
  // Patch 51-2 # 263
  constant_a = -8;
  result = ((size / constant_a) <= i);
  uni_klee_add_patch(patch_results, 263, result);
  // Patch 51-3 # 264
  constant_a = -7;
  result = ((size / constant_a) <= i);
  uni_klee_add_patch(patch_results, 264, result);
  // Patch 51-4 # 265
  constant_a = -6;
  result = ((size / constant_a) <= i);
  uni_klee_add_patch(patch_results, 265, result);
  // Patch 51-5 # 266
  constant_a = -5;
  result = ((size / constant_a) <= i);
  uni_klee_add_patch(patch_results, 266, result);
  // Patch 51-6 # 267
  constant_a = -4;
  result = ((size / constant_a) <= i);
  uni_klee_add_patch(patch_results, 267, result);
  // Patch 51-7 # 268
  constant_a = -3;
  result = ((size / constant_a) <= i);
  uni_klee_add_patch(patch_results, 268, result);
  // Patch 51-8 # 269
  constant_a = -2;
  result = ((size / constant_a) <= i);
  uni_klee_add_patch(patch_results, 269, result);
  // Patch 51-9 # 270
  constant_a = -1;
  result = ((size / constant_a) <= i);
  uni_klee_add_patch(patch_results, 270, result);
  // Patch 51-10 # 271
  constant_a = 9;
  result = ((size / constant_a) <= i);
  uni_klee_add_patch(patch_results, 271, result);
  // Patch 51-11 # 272
  constant_a = 10;
  result = ((size / constant_a) <= i);
  uni_klee_add_patch(patch_results, 272, result);
  // Patch 52-0 # 273
  constant_a = -10;
  result = (constant_a <= (size / i));
  uni_klee_add_patch(patch_results, 273, result);
  // Patch 52-1 # 274
  constant_a = -9;
  result = (constant_a <= (size / i));
  uni_klee_add_patch(patch_results, 274, result);
  // Patch 52-2 # 275
  constant_a = -8;
  result = (constant_a <= (size / i));
  uni_klee_add_patch(patch_results, 275, result);
  // Patch 52-3 # 276
  constant_a = -7;
  result = (constant_a <= (size / i));
  uni_klee_add_patch(patch_results, 276, result);
  // Patch 52-4 # 277
  constant_a = -6;
  result = (constant_a <= (size / i));
  uni_klee_add_patch(patch_results, 277, result);
  // Patch 52-5 # 278
  constant_a = -5;
  result = (constant_a <= (size / i));
  uni_klee_add_patch(patch_results, 278, result);
  // Patch 52-6 # 279
  constant_a = -4;
  result = (constant_a <= (size / i));
  uni_klee_add_patch(patch_results, 279, result);
  // Patch 52-7 # 280
  constant_a = -3;
  result = (constant_a <= (size / i));
  uni_klee_add_patch(patch_results, 280, result);
  // Patch 52-8 # 281
  constant_a = -2;
  result = (constant_a <= (size / i));
  uni_klee_add_patch(patch_results, 281, result);
  // Patch 52-9 # 282
  constant_a = -1;
  result = (constant_a <= (size / i));
  uni_klee_add_patch(patch_results, 282, result);
  // Patch 52-10 # 283
  constant_a = 0;
  result = (constant_a <= (size / i));
  uni_klee_add_patch(patch_results, 283, result);
  // Patch 52-11 # 284
  constant_a = 1;
  result = (constant_a <= (size / i));
  uni_klee_add_patch(patch_results, 284, result);
  // Patch 52-12 # 285
  constant_a = 2;
  result = (constant_a <= (size / i));
  uni_klee_add_patch(patch_results, 285, result);
  // Patch 53-0 # 286
  constant_a = -10;
  result = (constant_a <= i);
  uni_klee_add_patch(patch_results, 286, result);
  // Patch 53-1 # 287
  constant_a = -9;
  result = (constant_a <= i);
  uni_klee_add_patch(patch_results, 287, result);
  // Patch 53-2 # 288
  constant_a = -8;
  result = (constant_a <= i);
  uni_klee_add_patch(patch_results, 288, result);
  // Patch 53-3 # 289
  constant_a = -7;
  result = (constant_a <= i);
  uni_klee_add_patch(patch_results, 289, result);
  // Patch 53-4 # 290
  constant_a = -6;
  result = (constant_a <= i);
  uni_klee_add_patch(patch_results, 290, result);
  // Patch 53-5 # 291
  constant_a = -5;
  result = (constant_a <= i);
  uni_klee_add_patch(patch_results, 291, result);
  // Patch 53-6 # 292
  constant_a = -4;
  result = (constant_a <= i);
  uni_klee_add_patch(patch_results, 292, result);
  // Patch 53-7 # 293
  constant_a = -3;
  result = (constant_a <= i);
  uni_klee_add_patch(patch_results, 293, result);
  // Patch 53-8 # 294
  constant_a = -2;
  result = (constant_a <= i);
  uni_klee_add_patch(patch_results, 294, result);
  // Patch 53-9 # 295
  constant_a = -1;
  result = (constant_a <= i);
  uni_klee_add_patch(patch_results, 295, result);
  // Patch 53-10 # 296
  constant_a = 0;
  result = (constant_a <= i);
  uni_klee_add_patch(patch_results, 296, result);
  // Patch 53-11 # 297
  constant_a = 1;
  result = (constant_a <= i);
  uni_klee_add_patch(patch_results, 297, result);
  // Patch 53-12 # 298
  constant_a = 2;
  result = (constant_a <= i);
  uni_klee_add_patch(patch_results, 298, result);
  // Patch 53-13 # 299
  constant_a = 3;
  result = (constant_a <= i);
  uni_klee_add_patch(patch_results, 299, result);
  // Patch 54-0 # 300
  constant_a = -10;
  result = (constant_a < size);
  uni_klee_add_patch(patch_results, 300, result);
  // Patch 54-1 # 301
  constant_a = -9;
  result = (constant_a < size);
  uni_klee_add_patch(patch_results, 301, result);
  // Patch 54-2 # 302
  constant_a = -8;
  result = (constant_a < size);
  uni_klee_add_patch(patch_results, 302, result);
  // Patch 54-3 # 303
  constant_a = -7;
  result = (constant_a < size);
  uni_klee_add_patch(patch_results, 303, result);
  // Patch 54-4 # 304
  constant_a = -6;
  result = (constant_a < size);
  uni_klee_add_patch(patch_results, 304, result);
  // Patch 54-5 # 305
  constant_a = -5;
  result = (constant_a < size);
  uni_klee_add_patch(patch_results, 305, result);
  // Patch 54-6 # 306
  constant_a = -4;
  result = (constant_a < size);
  uni_klee_add_patch(patch_results, 306, result);
  // Patch 54-7 # 307
  constant_a = -3;
  result = (constant_a < size);
  uni_klee_add_patch(patch_results, 307, result);
  // Patch 54-8 # 308
  constant_a = -2;
  result = (constant_a < size);
  uni_klee_add_patch(patch_results, 308, result);
  // Patch 54-9 # 309
  constant_a = -1;
  result = (constant_a < size);
  uni_klee_add_patch(patch_results, 309, result);
  // Patch 54-10 # 310
  constant_a = 0;
  result = (constant_a < size);
  uni_klee_add_patch(patch_results, 310, result);
  // Patch 54-11 # 311
  constant_a = 1;
  result = (constant_a < size);
  uni_klee_add_patch(patch_results, 311, result);
  // Patch 54-12 # 312
  constant_a = 2;
  result = (constant_a < size);
  uni_klee_add_patch(patch_results, 312, result);
  // Patch 54-13 # 313
  constant_a = 3;
  result = (constant_a < size);
  uni_klee_add_patch(patch_results, 313, result);
  // Patch 54-14 # 314
  constant_a = 4;
  result = (constant_a < size);
  uni_klee_add_patch(patch_results, 314, result);
  // Patch 54-15 # 315
  constant_a = 5;
  result = (constant_a < size);
  uni_klee_add_patch(patch_results, 315, result);
  // Patch 54-16 # 316
  constant_a = 6;
  result = (constant_a < size);
  uni_klee_add_patch(patch_results, 316, result);
  // Patch 55-0 # 317
  constant_a = -10;
  result = ((size / constant_a) != i);
  uni_klee_add_patch(patch_results, 317, result);
  // Patch 55-1 # 318
  constant_a = -9;
  result = ((size / constant_a) != i);
  uni_klee_add_patch(patch_results, 318, result);
  // Patch 55-2 # 319
  constant_a = -8;
  result = ((size / constant_a) != i);
  uni_klee_add_patch(patch_results, 319, result);
  // Patch 55-3 # 320
  constant_a = -7;
  result = ((size / constant_a) != i);
  uni_klee_add_patch(patch_results, 320, result);
  // Patch 55-4 # 321
  constant_a = -6;
  result = ((size / constant_a) != i);
  uni_klee_add_patch(patch_results, 321, result);
  // Patch 55-5 # 322
  constant_a = -5;
  result = ((size / constant_a) != i);
  uni_klee_add_patch(patch_results, 322, result);
  // Patch 55-6 # 323
  constant_a = -4;
  result = ((size / constant_a) != i);
  uni_klee_add_patch(patch_results, 323, result);
  // Patch 55-7 # 324
  constant_a = -3;
  result = ((size / constant_a) != i);
  uni_klee_add_patch(patch_results, 324, result);
  // Patch 55-8 # 325
  constant_a = -2;
  result = ((size / constant_a) != i);
  uni_klee_add_patch(patch_results, 325, result);
  // Patch 55-9 # 326
  constant_a = -1;
  result = ((size / constant_a) != i);
  uni_klee_add_patch(patch_results, 326, result);
  // Patch 55-10 # 327
  constant_a = 1;
  result = ((size / constant_a) != i);
  uni_klee_add_patch(patch_results, 327, result);
  // Patch 55-11 # 328
  constant_a = 3;
  result = ((size / constant_a) != i);
  uni_klee_add_patch(patch_results, 328, result);
  // Patch 55-12 # 329
  constant_a = 4;
  result = ((size / constant_a) != i);
  uni_klee_add_patch(patch_results, 329, result);
  // Patch 55-13 # 330
  constant_a = 6;
  result = ((size / constant_a) != i);
  uni_klee_add_patch(patch_results, 330, result);
  // Patch 55-14 # 331
  constant_a = 7;
  result = ((size / constant_a) != i);
  uni_klee_add_patch(patch_results, 331, result);
  // Patch 55-15 # 332
  constant_a = 8;
  result = ((size / constant_a) != i);
  uni_klee_add_patch(patch_results, 332, result);
  // Patch 56-0 # 333
  constant_a = -10;
  result = (constant_a <= size);
  uni_klee_add_patch(patch_results, 333, result);
  // Patch 56-1 # 334
  constant_a = -9;
  result = (constant_a <= size);
  uni_klee_add_patch(patch_results, 334, result);
  // Patch 56-2 # 335
  constant_a = -8;
  result = (constant_a <= size);
  uni_klee_add_patch(patch_results, 335, result);
  // Patch 56-3 # 336
  constant_a = -7;
  result = (constant_a <= size);
  uni_klee_add_patch(patch_results, 336, result);
  // Patch 56-4 # 337
  constant_a = -6;
  result = (constant_a <= size);
  uni_klee_add_patch(patch_results, 337, result);
  // Patch 56-5 # 338
  constant_a = -5;
  result = (constant_a <= size);
  uni_klee_add_patch(patch_results, 338, result);
  // Patch 56-6 # 339
  constant_a = -4;
  result = (constant_a <= size);
  uni_klee_add_patch(patch_results, 339, result);
  // Patch 56-7 # 340
  constant_a = -3;
  result = (constant_a <= size);
  uni_klee_add_patch(patch_results, 340, result);
  // Patch 56-8 # 341
  constant_a = -2;
  result = (constant_a <= size);
  uni_klee_add_patch(patch_results, 341, result);
  // Patch 56-9 # 342
  constant_a = -1;
  result = (constant_a <= size);
  uni_klee_add_patch(patch_results, 342, result);
  // Patch 56-10 # 343
  constant_a = 0;
  result = (constant_a <= size);
  uni_klee_add_patch(patch_results, 343, result);
  // Patch 56-11 # 344
  constant_a = 1;
  result = (constant_a <= size);
  uni_klee_add_patch(patch_results, 344, result);
  // Patch 56-12 # 345
  constant_a = 2;
  result = (constant_a <= size);
  uni_klee_add_patch(patch_results, 345, result);
  // Patch 56-13 # 346
  constant_a = 3;
  result = (constant_a <= size);
  uni_klee_add_patch(patch_results, 346, result);
  // Patch 56-14 # 347
  constant_a = 4;
  result = (constant_a <= size);
  uni_klee_add_patch(patch_results, 347, result);
  // Patch 56-15 # 348
  constant_a = 5;
  result = (constant_a <= size);
  uni_klee_add_patch(patch_results, 348, result);
  // Patch 56-16 # 349
  constant_a = 6;
  result = (constant_a <= size);
  uni_klee_add_patch(patch_results, 349, result);
  // Patch 56-17 # 350
  constant_a = 7;
  result = (constant_a <= size);
  uni_klee_add_patch(patch_results, 350, result);
  // Patch 57-0 # 351
  constant_a = -10;
  result = ((size / i) != constant_a);
  uni_klee_add_patch(patch_results, 351, result);
  // Patch 57-1 # 352
  constant_a = -9;
  result = ((size / i) != constant_a);
  uni_klee_add_patch(patch_results, 352, result);
  // Patch 57-2 # 353
  constant_a = -8;
  result = ((size / i) != constant_a);
  uni_klee_add_patch(patch_results, 353, result);
  // Patch 57-3 # 354
  constant_a = -7;
  result = ((size / i) != constant_a);
  uni_klee_add_patch(patch_results, 354, result);
  // Patch 57-4 # 355
  constant_a = -6;
  result = ((size / i) != constant_a);
  uni_klee_add_patch(patch_results, 355, result);
  // Patch 57-5 # 356
  constant_a = -5;
  result = ((size / i) != constant_a);
  uni_klee_add_patch(patch_results, 356, result);
  // Patch 57-6 # 357
  constant_a = -4;
  result = ((size / i) != constant_a);
  uni_klee_add_patch(patch_results, 357, result);
  // Patch 57-7 # 358
  constant_a = -3;
  result = ((size / i) != constant_a);
  uni_klee_add_patch(patch_results, 358, result);
  // Patch 57-8 # 359
  constant_a = -2;
  result = ((size / i) != constant_a);
  uni_klee_add_patch(patch_results, 359, result);
  // Patch 57-9 # 360
  constant_a = -1;
  result = ((size / i) != constant_a);
  uni_klee_add_patch(patch_results, 360, result);
  // Patch 57-10 # 361
  constant_a = 0;
  result = ((size / i) != constant_a);
  uni_klee_add_patch(patch_results, 361, result);
  // Patch 57-11 # 362
  constant_a = 1;
  result = ((size / i) != constant_a);
  uni_klee_add_patch(patch_results, 362, result);
  // Patch 57-12 # 363
  constant_a = 3;
  result = ((size / i) != constant_a);
  uni_klee_add_patch(patch_results, 363, result);
  // Patch 57-13 # 364
  constant_a = 4;
  result = ((size / i) != constant_a);
  uni_klee_add_patch(patch_results, 364, result);
  // Patch 57-14 # 365
  constant_a = 6;
  result = ((size / i) != constant_a);
  uni_klee_add_patch(patch_results, 365, result);
  // Patch 57-15 # 366
  constant_a = 7;
  result = ((size / i) != constant_a);
  uni_klee_add_patch(patch_results, 366, result);
  // Patch 57-16 # 367
  constant_a = 8;
  result = ((size / i) != constant_a);
  uni_klee_add_patch(patch_results, 367, result);
  // Patch 57-17 # 368
  constant_a = 9;
  result = ((size / i) != constant_a);
  uni_klee_add_patch(patch_results, 368, result);
  // Patch 58-0 # 369
  constant_a = -10;
  result = (constant_a != i);
  uni_klee_add_patch(patch_results, 369, result);
  // Patch 58-1 # 370
  constant_a = -9;
  result = (constant_a != i);
  uni_klee_add_patch(patch_results, 370, result);
  // Patch 58-2 # 371
  constant_a = -8;
  result = (constant_a != i);
  uni_klee_add_patch(patch_results, 371, result);
  // Patch 58-3 # 372
  constant_a = -7;
  result = (constant_a != i);
  uni_klee_add_patch(patch_results, 372, result);
  // Patch 58-4 # 373
  constant_a = -6;
  result = (constant_a != i);
  uni_klee_add_patch(patch_results, 373, result);
  // Patch 58-5 # 374
  constant_a = -5;
  result = (constant_a != i);
  uni_klee_add_patch(patch_results, 374, result);
  // Patch 58-6 # 375
  constant_a = -4;
  result = (constant_a != i);
  uni_klee_add_patch(patch_results, 375, result);
  // Patch 58-7 # 376
  constant_a = -3;
  result = (constant_a != i);
  uni_klee_add_patch(patch_results, 376, result);
  // Patch 58-8 # 377
  constant_a = -2;
  result = (constant_a != i);
  uni_klee_add_patch(patch_results, 377, result);
  // Patch 58-9 # 378
  constant_a = -1;
  result = (constant_a != i);
  uni_klee_add_patch(patch_results, 378, result);
  // Patch 58-10 # 379
  constant_a = 0;
  result = (constant_a != i);
  uni_klee_add_patch(patch_results, 379, result);
  // Patch 58-11 # 380
  constant_a = 1;
  result = (constant_a != i);
  uni_klee_add_patch(patch_results, 380, result);
  // Patch 58-12 # 381
  constant_a = 2;
  result = (constant_a != i);
  uni_klee_add_patch(patch_results, 381, result);
  // Patch 58-13 # 382
  constant_a = 4;
  result = (constant_a != i);
  uni_klee_add_patch(patch_results, 382, result);
  // Patch 58-14 # 383
  constant_a = 5;
  result = (constant_a != i);
  uni_klee_add_patch(patch_results, 383, result);
  // Patch 58-15 # 384
  constant_a = 7;
  result = (constant_a != i);
  uni_klee_add_patch(patch_results, 384, result);
  // Patch 58-16 # 385
  constant_a = 8;
  result = (constant_a != i);
  uni_klee_add_patch(patch_results, 385, result);
  // Patch 58-17 # 386
  constant_a = 9;
  result = (constant_a != i);
  uni_klee_add_patch(patch_results, 386, result);
  // Patch 58-18 # 387
  constant_a = 10;
  result = (constant_a != i);
  uni_klee_add_patch(patch_results, 387, result);
  // Patch 59-0 # 388
  constant_a = -10;
  result = ((constant_a / i) < i);
  uni_klee_add_patch(patch_results, 388, result);
  // Patch 59-1 # 389
  constant_a = -9;
  result = ((constant_a / i) < i);
  uni_klee_add_patch(patch_results, 389, result);
  // Patch 59-2 # 390
  constant_a = -8;
  result = ((constant_a / i) < i);
  uni_klee_add_patch(patch_results, 390, result);
  // Patch 59-3 # 391
  constant_a = -7;
  result = ((constant_a / i) < i);
  uni_klee_add_patch(patch_results, 391, result);
  // Patch 59-4 # 392
  constant_a = -6;
  result = ((constant_a / i) < i);
  uni_klee_add_patch(patch_results, 392, result);
  // Patch 59-5 # 393
  constant_a = -5;
  result = ((constant_a / i) < i);
  uni_klee_add_patch(patch_results, 393, result);
  // Patch 59-6 # 394
  constant_a = -4;
  result = ((constant_a / i) < i);
  uni_klee_add_patch(patch_results, 394, result);
  // Patch 59-7 # 395
  constant_a = -3;
  result = ((constant_a / i) < i);
  uni_klee_add_patch(patch_results, 395, result);
  // Patch 59-8 # 396
  constant_a = -2;
  result = ((constant_a / i) < i);
  uni_klee_add_patch(patch_results, 396, result);
  // Patch 59-9 # 397
  constant_a = -1;
  result = ((constant_a / i) < i);
  uni_klee_add_patch(patch_results, 397, result);
  // Patch 59-10 # 398
  constant_a = 0;
  result = ((constant_a / i) < i);
  uni_klee_add_patch(patch_results, 398, result);
  // Patch 59-11 # 399
  constant_a = 1;
  result = ((constant_a / i) < i);
  uni_klee_add_patch(patch_results, 399, result);
  // Patch 59-12 # 400
  constant_a = 2;
  result = ((constant_a / i) < i);
  uni_klee_add_patch(patch_results, 400, result);
  // Patch 59-13 # 401
  constant_a = 3;
  result = ((constant_a / i) < i);
  uni_klee_add_patch(patch_results, 401, result);
  // Patch 59-14 # 402
  constant_a = 4;
  result = ((constant_a / i) < i);
  uni_klee_add_patch(patch_results, 402, result);
  // Patch 59-15 # 403
  constant_a = 5;
  result = ((constant_a / i) < i);
  uni_klee_add_patch(patch_results, 403, result);
  // Patch 59-16 # 404
  constant_a = 6;
  result = ((constant_a / i) < i);
  uni_klee_add_patch(patch_results, 404, result);
  // Patch 59-17 # 405
  constant_a = 7;
  result = ((constant_a / i) < i);
  uni_klee_add_patch(patch_results, 405, result);
  // Patch 59-18 # 406
  constant_a = 8;
  result = ((constant_a / i) < i);
  uni_klee_add_patch(patch_results, 406, result);
  // Patch 60-0 # 407
  constant_a = -10;
  result = ((constant_a / i) != i);
  uni_klee_add_patch(patch_results, 407, result);
  // Patch 60-1 # 408
  constant_a = -9;
  result = ((constant_a / i) != i);
  uni_klee_add_patch(patch_results, 408, result);
  // Patch 60-2 # 409
  constant_a = -8;
  result = ((constant_a / i) != i);
  uni_klee_add_patch(patch_results, 409, result);
  // Patch 60-3 # 410
  constant_a = -7;
  result = ((constant_a / i) != i);
  uni_klee_add_patch(patch_results, 410, result);
  // Patch 60-4 # 411
  constant_a = -6;
  result = ((constant_a / i) != i);
  uni_klee_add_patch(patch_results, 411, result);
  // Patch 60-5 # 412
  constant_a = -5;
  result = ((constant_a / i) != i);
  uni_klee_add_patch(patch_results, 412, result);
  // Patch 60-6 # 413
  constant_a = -4;
  result = ((constant_a / i) != i);
  uni_klee_add_patch(patch_results, 413, result);
  // Patch 60-7 # 414
  constant_a = -3;
  result = ((constant_a / i) != i);
  uni_klee_add_patch(patch_results, 414, result);
  // Patch 60-8 # 415
  constant_a = -2;
  result = ((constant_a / i) != i);
  uni_klee_add_patch(patch_results, 415, result);
  // Patch 60-9 # 416
  constant_a = -1;
  result = ((constant_a / i) != i);
  uni_klee_add_patch(patch_results, 416, result);
  // Patch 60-10 # 417
  constant_a = 0;
  result = ((constant_a / i) != i);
  uni_klee_add_patch(patch_results, 417, result);
  // Patch 60-11 # 418
  constant_a = 1;
  result = ((constant_a / i) != i);
  uni_klee_add_patch(patch_results, 418, result);
  // Patch 60-12 # 419
  constant_a = 2;
  result = ((constant_a / i) != i);
  uni_klee_add_patch(patch_results, 419, result);
  // Patch 60-13 # 420
  constant_a = 3;
  result = ((constant_a / i) != i);
  uni_klee_add_patch(patch_results, 420, result);
  // Patch 60-14 # 421
  constant_a = 4;
  result = ((constant_a / i) != i);
  uni_klee_add_patch(patch_results, 421, result);
  // Patch 60-15 # 422
  constant_a = 5;
  result = ((constant_a / i) != i);
  uni_klee_add_patch(patch_results, 422, result);
  // Patch 60-16 # 423
  constant_a = 6;
  result = ((constant_a / i) != i);
  uni_klee_add_patch(patch_results, 423, result);
  // Patch 60-17 # 424
  constant_a = 7;
  result = ((constant_a / i) != i);
  uni_klee_add_patch(patch_results, 424, result);
  // Patch 60-18 # 425
  constant_a = 8;
  result = ((constant_a / i) != i);
  uni_klee_add_patch(patch_results, 425, result);
  // Patch 61-0 # 426
  constant_a = -10;
  result = (constant_a != size);
  uni_klee_add_patch(patch_results, 426, result);
  // Patch 61-1 # 427
  constant_a = -9;
  result = (constant_a != size);
  uni_klee_add_patch(patch_results, 427, result);
  // Patch 61-2 # 428
  constant_a = -8;
  result = (constant_a != size);
  uni_klee_add_patch(patch_results, 428, result);
  // Patch 61-3 # 429
  constant_a = -7;
  result = (constant_a != size);
  uni_klee_add_patch(patch_results, 429, result);
  // Patch 61-4 # 430
  constant_a = -6;
  result = (constant_a != size);
  uni_klee_add_patch(patch_results, 430, result);
  // Patch 61-5 # 431
  constant_a = -5;
  result = (constant_a != size);
  uni_klee_add_patch(patch_results, 431, result);
  // Patch 61-6 # 432
  constant_a = -4;
  result = (constant_a != size);
  uni_klee_add_patch(patch_results, 432, result);
  // Patch 61-7 # 433
  constant_a = -3;
  result = (constant_a != size);
  uni_klee_add_patch(patch_results, 433, result);
  // Patch 61-8 # 434
  constant_a = -2;
  result = (constant_a != size);
  uni_klee_add_patch(patch_results, 434, result);
  // Patch 61-9 # 435
  constant_a = -1;
  result = (constant_a != size);
  uni_klee_add_patch(patch_results, 435, result);
  // Patch 61-10 # 436
  constant_a = 0;
  result = (constant_a != size);
  uni_klee_add_patch(patch_results, 436, result);
  // Patch 61-11 # 437
  constant_a = 1;
  result = (constant_a != size);
  uni_klee_add_patch(patch_results, 437, result);
  // Patch 61-12 # 438
  constant_a = 2;
  result = (constant_a != size);
  uni_klee_add_patch(patch_results, 438, result);
  // Patch 61-13 # 439
  constant_a = 3;
  result = (constant_a != size);
  uni_klee_add_patch(patch_results, 439, result);
  // Patch 61-14 # 440
  constant_a = 4;
  result = (constant_a != size);
  uni_klee_add_patch(patch_results, 440, result);
  // Patch 61-15 # 441
  constant_a = 5;
  result = (constant_a != size);
  uni_klee_add_patch(patch_results, 441, result);
  // Patch 61-16 # 442
  constant_a = 6;
  result = (constant_a != size);
  uni_klee_add_patch(patch_results, 442, result);
  // Patch 61-17 # 443
  constant_a = 8;
  result = (constant_a != size);
  uni_klee_add_patch(patch_results, 443, result);
  // Patch 61-18 # 444
  constant_a = 9;
  result = (constant_a != size);
  uni_klee_add_patch(patch_results, 444, result);
  // Patch 61-19 # 445
  constant_a = 10;
  result = (constant_a != size);
  uni_klee_add_patch(patch_results, 445, result);
  // Patch 62-0 # 446
  constant_a = -10;
  result = ((constant_a / i) != constant_a);
  uni_klee_add_patch(patch_results, 446, result);
  // Patch 62-1 # 447
  constant_a = -9;
  result = ((constant_a / i) != constant_a);
  uni_klee_add_patch(patch_results, 447, result);
  // Patch 62-2 # 448
  constant_a = -8;
  result = ((constant_a / i) != constant_a);
  uni_klee_add_patch(patch_results, 448, result);
  // Patch 62-3 # 449
  constant_a = -7;
  result = ((constant_a / i) != constant_a);
  uni_klee_add_patch(patch_results, 449, result);
  // Patch 62-4 # 450
  constant_a = -6;
  result = ((constant_a / i) != constant_a);
  uni_klee_add_patch(patch_results, 450, result);
  // Patch 62-5 # 451
  constant_a = -5;
  result = ((constant_a / i) != constant_a);
  uni_klee_add_patch(patch_results, 451, result);
  // Patch 62-6 # 452
  constant_a = -4;
  result = ((constant_a / i) != constant_a);
  uni_klee_add_patch(patch_results, 452, result);
  // Patch 62-7 # 453
  constant_a = -3;
  result = ((constant_a / i) != constant_a);
  uni_klee_add_patch(patch_results, 453, result);
  // Patch 62-8 # 454
  constant_a = -2;
  result = ((constant_a / i) != constant_a);
  uni_klee_add_patch(patch_results, 454, result);
  // Patch 62-9 # 455
  constant_a = -1;
  result = ((constant_a / i) != constant_a);
  uni_klee_add_patch(patch_results, 455, result);
  // Patch 62-10 # 456
  constant_a = 1;
  result = ((constant_a / i) != constant_a);
  uni_klee_add_patch(patch_results, 456, result);
  // Patch 62-11 # 457
  constant_a = 2;
  result = ((constant_a / i) != constant_a);
  uni_klee_add_patch(patch_results, 457, result);
  // Patch 62-12 # 458
  constant_a = 3;
  result = ((constant_a / i) != constant_a);
  uni_klee_add_patch(patch_results, 458, result);
  // Patch 62-13 # 459
  constant_a = 4;
  result = ((constant_a / i) != constant_a);
  uni_klee_add_patch(patch_results, 459, result);
  // Patch 62-14 # 460
  constant_a = 5;
  result = ((constant_a / i) != constant_a);
  uni_klee_add_patch(patch_results, 460, result);
  // Patch 62-15 # 461
  constant_a = 6;
  result = ((constant_a / i) != constant_a);
  uni_klee_add_patch(patch_results, 461, result);
  // Patch 62-16 # 462
  constant_a = 7;
  result = ((constant_a / i) != constant_a);
  uni_klee_add_patch(patch_results, 462, result);
  // Patch 62-17 # 463
  constant_a = 8;
  result = ((constant_a / i) != constant_a);
  uni_klee_add_patch(patch_results, 463, result);
  // Patch 62-18 # 464
  constant_a = 9;
  result = ((constant_a / i) != constant_a);
  uni_klee_add_patch(patch_results, 464, result);
  // Patch 62-19 # 465
  constant_a = 10;
  result = ((constant_a / i) != constant_a);
  uni_klee_add_patch(patch_results, 465, result);
  // Patch 63-0 # 466
  constant_a = -10;
  result = ((constant_a / size) != constant_a);
  uni_klee_add_patch(patch_results, 466, result);
  // Patch 63-1 # 467
  constant_a = -9;
  result = ((constant_a / size) != constant_a);
  uni_klee_add_patch(patch_results, 467, result);
  // Patch 63-2 # 468
  constant_a = -8;
  result = ((constant_a / size) != constant_a);
  uni_klee_add_patch(patch_results, 468, result);
  // Patch 63-3 # 469
  constant_a = -7;
  result = ((constant_a / size) != constant_a);
  uni_klee_add_patch(patch_results, 469, result);
  // Patch 63-4 # 470
  constant_a = -6;
  result = ((constant_a / size) != constant_a);
  uni_klee_add_patch(patch_results, 470, result);
  // Patch 63-5 # 471
  constant_a = -5;
  result = ((constant_a / size) != constant_a);
  uni_klee_add_patch(patch_results, 471, result);
  // Patch 63-6 # 472
  constant_a = -4;
  result = ((constant_a / size) != constant_a);
  uni_klee_add_patch(patch_results, 472, result);
  // Patch 63-7 # 473
  constant_a = -3;
  result = ((constant_a / size) != constant_a);
  uni_klee_add_patch(patch_results, 473, result);
  // Patch 63-8 # 474
  constant_a = -2;
  result = ((constant_a / size) != constant_a);
  uni_klee_add_patch(patch_results, 474, result);
  // Patch 63-9 # 475
  constant_a = -1;
  result = ((constant_a / size) != constant_a);
  uni_klee_add_patch(patch_results, 475, result);
  // Patch 63-10 # 476
  constant_a = 1;
  result = ((constant_a / size) != constant_a);
  uni_klee_add_patch(patch_results, 476, result);
  // Patch 63-11 # 477
  constant_a = 2;
  result = ((constant_a / size) != constant_a);
  uni_klee_add_patch(patch_results, 477, result);
  // Patch 63-12 # 478
  constant_a = 3;
  result = ((constant_a / size) != constant_a);
  uni_klee_add_patch(patch_results, 478, result);
  // Patch 63-13 # 479
  constant_a = 4;
  result = ((constant_a / size) != constant_a);
  uni_klee_add_patch(patch_results, 479, result);
  // Patch 63-14 # 480
  constant_a = 5;
  result = ((constant_a / size) != constant_a);
  uni_klee_add_patch(patch_results, 480, result);
  // Patch 63-15 # 481
  constant_a = 6;
  result = ((constant_a / size) != constant_a);
  uni_klee_add_patch(patch_results, 481, result);
  // Patch 63-16 # 482
  constant_a = 7;
  result = ((constant_a / size) != constant_a);
  uni_klee_add_patch(patch_results, 482, result);
  // Patch 63-17 # 483
  constant_a = 8;
  result = ((constant_a / size) != constant_a);
  uni_klee_add_patch(patch_results, 483, result);
  // Patch 63-18 # 484
  constant_a = 9;
  result = ((constant_a / size) != constant_a);
  uni_klee_add_patch(patch_results, 484, result);
  // Patch 63-19 # 485
  constant_a = 10;
  result = ((constant_a / size) != constant_a);
  uni_klee_add_patch(patch_results, 485, result);
  // Patch 64-0 # 486
  constant_a = -10;
  result = ((i / constant_a) < i);
  uni_klee_add_patch(patch_results, 486, result);
  // Patch 64-1 # 487
  constant_a = -9;
  result = ((i / constant_a) < i);
  uni_klee_add_patch(patch_results, 487, result);
  // Patch 64-2 # 488
  constant_a = -8;
  result = ((i / constant_a) < i);
  uni_klee_add_patch(patch_results, 488, result);
  // Patch 64-3 # 489
  constant_a = -7;
  result = ((i / constant_a) < i);
  uni_klee_add_patch(patch_results, 489, result);
  // Patch 64-4 # 490
  constant_a = -6;
  result = ((i / constant_a) < i);
  uni_klee_add_patch(patch_results, 490, result);
  // Patch 64-5 # 491
  constant_a = -5;
  result = ((i / constant_a) < i);
  uni_klee_add_patch(patch_results, 491, result);
  // Patch 64-6 # 492
  constant_a = -4;
  result = ((i / constant_a) < i);
  uni_klee_add_patch(patch_results, 492, result);
  // Patch 64-7 # 493
  constant_a = -3;
  result = ((i / constant_a) < i);
  uni_klee_add_patch(patch_results, 493, result);
  // Patch 64-8 # 494
  constant_a = -2;
  result = ((i / constant_a) < i);
  uni_klee_add_patch(patch_results, 494, result);
  // Patch 64-9 # 495
  constant_a = -1;
  result = ((i / constant_a) < i);
  uni_klee_add_patch(patch_results, 495, result);
  // Patch 64-10 # 496
  constant_a = 2;
  result = ((i / constant_a) < i);
  uni_klee_add_patch(patch_results, 496, result);
  // Patch 64-11 # 497
  constant_a = 3;
  result = ((i / constant_a) < i);
  uni_klee_add_patch(patch_results, 497, result);
  // Patch 64-12 # 498
  constant_a = 4;
  result = ((i / constant_a) < i);
  uni_klee_add_patch(patch_results, 498, result);
  // Patch 64-13 # 499
  constant_a = 5;
  result = ((i / constant_a) < i);
  uni_klee_add_patch(patch_results, 499, result);
  // Patch 64-14 # 500
  constant_a = 6;
  result = ((i / constant_a) < i);
  uni_klee_add_patch(patch_results, 500, result);
  // Patch 64-15 # 501
  constant_a = 7;
  result = ((i / constant_a) < i);
  uni_klee_add_patch(patch_results, 501, result);
  // Patch 64-16 # 502
  constant_a = 8;
  result = ((i / constant_a) < i);
  uni_klee_add_patch(patch_results, 502, result);
  // Patch 64-17 # 503
  constant_a = 9;
  result = ((i / constant_a) < i);
  uni_klee_add_patch(patch_results, 503, result);
  // Patch 64-18 # 504
  constant_a = 10;
  result = ((i / constant_a) < i);
  uni_klee_add_patch(patch_results, 504, result);
  // Patch 65-0 # 505
  constant_a = -10;
  result = ((i / constant_a) != i);
  uni_klee_add_patch(patch_results, 505, result);
  // Patch 65-1 # 506
  constant_a = -9;
  result = ((i / constant_a) != i);
  uni_klee_add_patch(patch_results, 506, result);
  // Patch 65-2 # 507
  constant_a = -8;
  result = ((i / constant_a) != i);
  uni_klee_add_patch(patch_results, 507, result);
  // Patch 65-3 # 508
  constant_a = -7;
  result = ((i / constant_a) != i);
  uni_klee_add_patch(patch_results, 508, result);
  // Patch 65-4 # 509
  constant_a = -6;
  result = ((i / constant_a) != i);
  uni_klee_add_patch(patch_results, 509, result);
  // Patch 65-5 # 510
  constant_a = -5;
  result = ((i / constant_a) != i);
  uni_klee_add_patch(patch_results, 510, result);
  // Patch 65-6 # 511
  constant_a = -4;
  result = ((i / constant_a) != i);
  uni_klee_add_patch(patch_results, 511, result);
  // Patch 65-7 # 512
  constant_a = -3;
  result = ((i / constant_a) != i);
  uni_klee_add_patch(patch_results, 512, result);
  // Patch 65-8 # 513
  constant_a = -2;
  result = ((i / constant_a) != i);
  uni_klee_add_patch(patch_results, 513, result);
  // Patch 65-9 # 514
  constant_a = -1;
  result = ((i / constant_a) != i);
  uni_klee_add_patch(patch_results, 514, result);
  // Patch 65-10 # 515
  constant_a = 2;
  result = ((i / constant_a) != i);
  uni_klee_add_patch(patch_results, 515, result);
  // Patch 65-11 # 516
  constant_a = 3;
  result = ((i / constant_a) != i);
  uni_klee_add_patch(patch_results, 516, result);
  // Patch 65-12 # 517
  constant_a = 4;
  result = ((i / constant_a) != i);
  uni_klee_add_patch(patch_results, 517, result);
  // Patch 65-13 # 518
  constant_a = 5;
  result = ((i / constant_a) != i);
  uni_klee_add_patch(patch_results, 518, result);
  // Patch 65-14 # 519
  constant_a = 6;
  result = ((i / constant_a) != i);
  uni_klee_add_patch(patch_results, 519, result);
  // Patch 65-15 # 520
  constant_a = 7;
  result = ((i / constant_a) != i);
  uni_klee_add_patch(patch_results, 520, result);
  // Patch 65-16 # 521
  constant_a = 8;
  result = ((i / constant_a) != i);
  uni_klee_add_patch(patch_results, 521, result);
  // Patch 65-17 # 522
  constant_a = 9;
  result = ((i / constant_a) != i);
  uni_klee_add_patch(patch_results, 522, result);
  // Patch 65-18 # 523
  constant_a = 10;
  result = ((i / constant_a) != i);
  uni_klee_add_patch(patch_results, 523, result);
  // Patch 66-0 # 524
  constant_a = -10;
  result = ((i / size) != constant_a);
  uni_klee_add_patch(patch_results, 524, result);
  // Patch 66-1 # 525
  constant_a = -9;
  result = ((i / size) != constant_a);
  uni_klee_add_patch(patch_results, 525, result);
  // Patch 66-2 # 526
  constant_a = -8;
  result = ((i / size) != constant_a);
  uni_klee_add_patch(patch_results, 526, result);
  // Patch 66-3 # 527
  constant_a = -7;
  result = ((i / size) != constant_a);
  uni_klee_add_patch(patch_results, 527, result);
  // Patch 66-4 # 528
  constant_a = -6;
  result = ((i / size) != constant_a);
  uni_klee_add_patch(patch_results, 528, result);
  // Patch 66-5 # 529
  constant_a = -5;
  result = ((i / size) != constant_a);
  uni_klee_add_patch(patch_results, 529, result);
  // Patch 66-6 # 530
  constant_a = -4;
  result = ((i / size) != constant_a);
  uni_klee_add_patch(patch_results, 530, result);
  // Patch 66-7 # 531
  constant_a = -3;
  result = ((i / size) != constant_a);
  uni_klee_add_patch(patch_results, 531, result);
  // Patch 66-8 # 532
  constant_a = -2;
  result = ((i / size) != constant_a);
  uni_klee_add_patch(patch_results, 532, result);
  // Patch 66-9 # 533
  constant_a = -1;
  result = ((i / size) != constant_a);
  uni_klee_add_patch(patch_results, 533, result);
  // Patch 66-10 # 534
  constant_a = 1;
  result = ((i / size) != constant_a);
  uni_klee_add_patch(patch_results, 534, result);
  // Patch 66-11 # 535
  constant_a = 2;
  result = ((i / size) != constant_a);
  uni_klee_add_patch(patch_results, 535, result);
  // Patch 66-12 # 536
  constant_a = 3;
  result = ((i / size) != constant_a);
  uni_klee_add_patch(patch_results, 536, result);
  // Patch 66-13 # 537
  constant_a = 4;
  result = ((i / size) != constant_a);
  uni_klee_add_patch(patch_results, 537, result);
  // Patch 66-14 # 538
  constant_a = 5;
  result = ((i / size) != constant_a);
  uni_klee_add_patch(patch_results, 538, result);
  // Patch 66-15 # 539
  constant_a = 6;
  result = ((i / size) != constant_a);
  uni_klee_add_patch(patch_results, 539, result);
  // Patch 66-16 # 540
  constant_a = 7;
  result = ((i / size) != constant_a);
  uni_klee_add_patch(patch_results, 540, result);
  // Patch 66-17 # 541
  constant_a = 8;
  result = ((i / size) != constant_a);
  uni_klee_add_patch(patch_results, 541, result);
  // Patch 66-18 # 542
  constant_a = 9;
  result = ((i / size) != constant_a);
  uni_klee_add_patch(patch_results, 542, result);
  // Patch 66-19 # 543
  constant_a = 10;
  result = ((i / size) != constant_a);
  uni_klee_add_patch(patch_results, 543, result);
  // Patch 67-0 # 544
  constant_a = -10;
  result = ((size / constant_a) < size);
  uni_klee_add_patch(patch_results, 544, result);
  // Patch 67-1 # 545
  constant_a = -9;
  result = ((size / constant_a) < size);
  uni_klee_add_patch(patch_results, 545, result);
  // Patch 67-2 # 546
  constant_a = -8;
  result = ((size / constant_a) < size);
  uni_klee_add_patch(patch_results, 546, result);
  // Patch 67-3 # 547
  constant_a = -7;
  result = ((size / constant_a) < size);
  uni_klee_add_patch(patch_results, 547, result);
  // Patch 67-4 # 548
  constant_a = -6;
  result = ((size / constant_a) < size);
  uni_klee_add_patch(patch_results, 548, result);
  // Patch 67-5 # 549
  constant_a = -5;
  result = ((size / constant_a) < size);
  uni_klee_add_patch(patch_results, 549, result);
  // Patch 67-6 # 550
  constant_a = -4;
  result = ((size / constant_a) < size);
  uni_klee_add_patch(patch_results, 550, result);
  // Patch 67-7 # 551
  constant_a = -3;
  result = ((size / constant_a) < size);
  uni_klee_add_patch(patch_results, 551, result);
  // Patch 67-8 # 552
  constant_a = -2;
  result = ((size / constant_a) < size);
  uni_klee_add_patch(patch_results, 552, result);
  // Patch 67-9 # 553
  constant_a = -1;
  result = ((size / constant_a) < size);
  uni_klee_add_patch(patch_results, 553, result);
  // Patch 67-10 # 554
  constant_a = 2;
  result = ((size / constant_a) < size);
  uni_klee_add_patch(patch_results, 554, result);
  // Patch 67-11 # 555
  constant_a = 3;
  result = ((size / constant_a) < size);
  uni_klee_add_patch(patch_results, 555, result);
  // Patch 67-12 # 556
  constant_a = 4;
  result = ((size / constant_a) < size);
  uni_klee_add_patch(patch_results, 556, result);
  // Patch 67-13 # 557
  constant_a = 5;
  result = ((size / constant_a) < size);
  uni_klee_add_patch(patch_results, 557, result);
  // Patch 67-14 # 558
  constant_a = 6;
  result = ((size / constant_a) < size);
  uni_klee_add_patch(patch_results, 558, result);
  // Patch 67-15 # 559
  constant_a = 7;
  result = ((size / constant_a) < size);
  uni_klee_add_patch(patch_results, 559, result);
  // Patch 67-16 # 560
  constant_a = 8;
  result = ((size / constant_a) < size);
  uni_klee_add_patch(patch_results, 560, result);
  // Patch 67-17 # 561
  constant_a = 9;
  result = ((size / constant_a) < size);
  uni_klee_add_patch(patch_results, 561, result);
  // Patch 67-18 # 562
  constant_a = 10;
  result = ((size / constant_a) < size);
  uni_klee_add_patch(patch_results, 562, result);
  // Patch 68-0 # 563
  constant_a = -10;
  result = ((size / constant_a) != size);
  uni_klee_add_patch(patch_results, 563, result);
  // Patch 68-1 # 564
  constant_a = -9;
  result = ((size / constant_a) != size);
  uni_klee_add_patch(patch_results, 564, result);
  // Patch 68-2 # 565
  constant_a = -8;
  result = ((size / constant_a) != size);
  uni_klee_add_patch(patch_results, 565, result);
  // Patch 68-3 # 566
  constant_a = -7;
  result = ((size / constant_a) != size);
  uni_klee_add_patch(patch_results, 566, result);
  // Patch 68-4 # 567
  constant_a = -6;
  result = ((size / constant_a) != size);
  uni_klee_add_patch(patch_results, 567, result);
  // Patch 68-5 # 568
  constant_a = -5;
  result = ((size / constant_a) != size);
  uni_klee_add_patch(patch_results, 568, result);
  // Patch 68-6 # 569
  constant_a = -4;
  result = ((size / constant_a) != size);
  uni_klee_add_patch(patch_results, 569, result);
  // Patch 68-7 # 570
  constant_a = -3;
  result = ((size / constant_a) != size);
  uni_klee_add_patch(patch_results, 570, result);
  // Patch 68-8 # 571
  constant_a = -2;
  result = ((size / constant_a) != size);
  uni_klee_add_patch(patch_results, 571, result);
  // Patch 68-9 # 572
  constant_a = -1;
  result = ((size / constant_a) != size);
  uni_klee_add_patch(patch_results, 572, result);
  // Patch 68-10 # 573
  constant_a = 2;
  result = ((size / constant_a) != size);
  uni_klee_add_patch(patch_results, 573, result);
  // Patch 68-11 # 574
  constant_a = 3;
  result = ((size / constant_a) != size);
  uni_klee_add_patch(patch_results, 574, result);
  // Patch 68-12 # 575
  constant_a = 4;
  result = ((size / constant_a) != size);
  uni_klee_add_patch(patch_results, 575, result);
  // Patch 68-13 # 576
  constant_a = 5;
  result = ((size / constant_a) != size);
  uni_klee_add_patch(patch_results, 576, result);
  // Patch 68-14 # 577
  constant_a = 6;
  result = ((size / constant_a) != size);
  uni_klee_add_patch(patch_results, 577, result);
  // Patch 68-15 # 578
  constant_a = 7;
  result = ((size / constant_a) != size);
  uni_klee_add_patch(patch_results, 578, result);
  // Patch 68-16 # 579
  constant_a = 8;
  result = ((size / constant_a) != size);
  uni_klee_add_patch(patch_results, 579, result);
  // Patch 68-17 # 580
  constant_a = 9;
  result = ((size / constant_a) != size);
  uni_klee_add_patch(patch_results, 580, result);
  // Patch 68-18 # 581
  constant_a = 10;
  result = ((size / constant_a) != size);
  uni_klee_add_patch(patch_results, 581, result);
  // Patch 69-0 # 582
  constant_a = -10;
  result = ((i / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 582, result);
  // Patch 69-1 # 583
  constant_a = -9;
  result = ((i / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 583, result);
  // Patch 69-2 # 584
  constant_a = -8;
  result = ((i / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 584, result);
  // Patch 69-3 # 585
  constant_a = -7;
  result = ((i / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 585, result);
  // Patch 69-4 # 586
  constant_a = -6;
  result = ((i / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 586, result);
  // Patch 69-5 # 587
  constant_a = -5;
  result = ((i / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 587, result);
  // Patch 69-6 # 588
  constant_a = -4;
  result = ((i / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 588, result);
  // Patch 69-7 # 589
  constant_a = -3;
  result = ((i / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 589, result);
  // Patch 69-8 # 590
  constant_a = -2;
  result = ((i / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 590, result);
  // Patch 69-9 # 591
  constant_a = -1;
  result = ((i / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 591, result);
  // Patch 69-10 # 592
  constant_a = 1;
  result = ((i / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 592, result);
  // Patch 69-11 # 593
  constant_a = 2;
  result = ((i / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 593, result);
  // Patch 69-12 # 594
  constant_a = 3;
  result = ((i / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 594, result);
  // Patch 69-13 # 595
  constant_a = 4;
  result = ((i / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 595, result);
  // Patch 69-14 # 596
  constant_a = 5;
  result = ((i / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 596, result);
  // Patch 69-15 # 597
  constant_a = 6;
  result = ((i / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 597, result);
  // Patch 69-16 # 598
  constant_a = 7;
  result = ((i / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 598, result);
  // Patch 69-17 # 599
  constant_a = 8;
  result = ((i / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 599, result);
  // Patch 69-18 # 600
  constant_a = 9;
  result = ((i / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 600, result);
  // Patch 69-19 # 601
  constant_a = 10;
  result = ((i / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 601, result);
  // Patch 70-0 # 602
  constant_a = -10;
  result = ((size / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 602, result);
  // Patch 70-1 # 603
  constant_a = -9;
  result = ((size / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 603, result);
  // Patch 70-2 # 604
  constant_a = -8;
  result = ((size / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 604, result);
  // Patch 70-3 # 605
  constant_a = -7;
  result = ((size / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 605, result);
  // Patch 70-4 # 606
  constant_a = -6;
  result = ((size / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 606, result);
  // Patch 70-5 # 607
  constant_a = -5;
  result = ((size / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 607, result);
  // Patch 70-6 # 608
  constant_a = -4;
  result = ((size / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 608, result);
  // Patch 70-7 # 609
  constant_a = -3;
  result = ((size / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 609, result);
  // Patch 70-8 # 610
  constant_a = -2;
  result = ((size / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 610, result);
  // Patch 70-9 # 611
  constant_a = -1;
  result = ((size / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 611, result);
  // Patch 70-10 # 612
  constant_a = 1;
  result = ((size / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 612, result);
  // Patch 70-11 # 613
  constant_a = 2;
  result = ((size / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 613, result);
  // Patch 70-12 # 614
  constant_a = 3;
  result = ((size / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 614, result);
  // Patch 70-13 # 615
  constant_a = 4;
  result = ((size / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 615, result);
  // Patch 70-14 # 616
  constant_a = 5;
  result = ((size / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 616, result);
  // Patch 70-15 # 617
  constant_a = 6;
  result = ((size / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 617, result);
  // Patch 70-16 # 618
  constant_a = 7;
  result = ((size / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 618, result);
  // Patch 70-17 # 619
  constant_a = 8;
  result = ((size / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 619, result);
  // Patch 70-18 # 620
  constant_a = 9;
  result = ((size / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 620, result);
  // Patch 70-19 # 621
  constant_a = 10;
  result = ((size / constant_a) != constant_a);
  uni_klee_add_patch(patch_results, 621, result);
  // Patch 71-0 # 622
  constant_a = -10;
  result = ((i / constant_a) <= i);
  uni_klee_add_patch(patch_results, 622, result);
  // Patch 71-1 # 623
  constant_a = -9;
  result = ((i / constant_a) <= i);
  uni_klee_add_patch(patch_results, 623, result);
  // Patch 71-2 # 624
  constant_a = -8;
  result = ((i / constant_a) <= i);
  uni_klee_add_patch(patch_results, 624, result);
  // Patch 71-3 # 625
  constant_a = -7;
  result = ((i / constant_a) <= i);
  uni_klee_add_patch(patch_results, 625, result);
  // Patch 71-4 # 626
  constant_a = -6;
  result = ((i / constant_a) <= i);
  uni_klee_add_patch(patch_results, 626, result);
  // Patch 71-5 # 627
  constant_a = -5;
  result = ((i / constant_a) <= i);
  uni_klee_add_patch(patch_results, 627, result);
  // Patch 71-6 # 628
  constant_a = -4;
  result = ((i / constant_a) <= i);
  uni_klee_add_patch(patch_results, 628, result);
  // Patch 71-7 # 629
  constant_a = -3;
  result = ((i / constant_a) <= i);
  uni_klee_add_patch(patch_results, 629, result);
  // Patch 71-8 # 630
  constant_a = -2;
  result = ((i / constant_a) <= i);
  uni_klee_add_patch(patch_results, 630, result);
  // Patch 71-9 # 631
  constant_a = -1;
  result = ((i / constant_a) <= i);
  uni_klee_add_patch(patch_results, 631, result);
  // Patch 71-10 # 632
  constant_a = 1;
  result = ((i / constant_a) <= i);
  uni_klee_add_patch(patch_results, 632, result);
  // Patch 71-11 # 633
  constant_a = 2;
  result = ((i / constant_a) <= i);
  uni_klee_add_patch(patch_results, 633, result);
  // Patch 71-12 # 634
  constant_a = 3;
  result = ((i / constant_a) <= i);
  uni_klee_add_patch(patch_results, 634, result);
  // Patch 71-13 # 635
  constant_a = 4;
  result = ((i / constant_a) <= i);
  uni_klee_add_patch(patch_results, 635, result);
  // Patch 71-14 # 636
  constant_a = 5;
  result = ((i / constant_a) <= i);
  uni_klee_add_patch(patch_results, 636, result);
  // Patch 71-15 # 637
  constant_a = 6;
  result = ((i / constant_a) <= i);
  uni_klee_add_patch(patch_results, 637, result);
  // Patch 71-16 # 638
  constant_a = 7;
  result = ((i / constant_a) <= i);
  uni_klee_add_patch(patch_results, 638, result);
  // Patch 71-17 # 639
  constant_a = 8;
  result = ((i / constant_a) <= i);
  uni_klee_add_patch(patch_results, 639, result);
  // Patch 71-18 # 640
  constant_a = 9;
  result = ((i / constant_a) <= i);
  uni_klee_add_patch(patch_results, 640, result);
  // Patch 71-19 # 641
  constant_a = 10;
  result = ((i / constant_a) <= i);
  uni_klee_add_patch(patch_results, 641, result);
  // Patch 72-0 # 642
  constant_a = -10;
  result = ((constant_a / i) <= i);
  uni_klee_add_patch(patch_results, 642, result);
  // Patch 72-1 # 643
  constant_a = -9;
  result = ((constant_a / i) <= i);
  uni_klee_add_patch(patch_results, 643, result);
  // Patch 72-2 # 644
  constant_a = -8;
  result = ((constant_a / i) <= i);
  uni_klee_add_patch(patch_results, 644, result);
  // Patch 72-3 # 645
  constant_a = -7;
  result = ((constant_a / i) <= i);
  uni_klee_add_patch(patch_results, 645, result);
  // Patch 72-4 # 646
  constant_a = -6;
  result = ((constant_a / i) <= i);
  uni_klee_add_patch(patch_results, 646, result);
  // Patch 72-5 # 647
  constant_a = -5;
  result = ((constant_a / i) <= i);
  uni_klee_add_patch(patch_results, 647, result);
  // Patch 72-6 # 648
  constant_a = -4;
  result = ((constant_a / i) <= i);
  uni_klee_add_patch(patch_results, 648, result);
  // Patch 72-7 # 649
  constant_a = -3;
  result = ((constant_a / i) <= i);
  uni_klee_add_patch(patch_results, 649, result);
  // Patch 72-8 # 650
  constant_a = -2;
  result = ((constant_a / i) <= i);
  uni_klee_add_patch(patch_results, 650, result);
  // Patch 72-9 # 651
  constant_a = -1;
  result = ((constant_a / i) <= i);
  uni_klee_add_patch(patch_results, 651, result);
  // Patch 72-10 # 652
  constant_a = 0;
  result = ((constant_a / i) <= i);
  uni_klee_add_patch(patch_results, 652, result);
  // Patch 72-11 # 653
  constant_a = 1;
  result = ((constant_a / i) <= i);
  uni_klee_add_patch(patch_results, 653, result);
  // Patch 72-12 # 654
  constant_a = 2;
  result = ((constant_a / i) <= i);
  uni_klee_add_patch(patch_results, 654, result);
  // Patch 72-13 # 655
  constant_a = 3;
  result = ((constant_a / i) <= i);
  uni_klee_add_patch(patch_results, 655, result);
  // Patch 72-14 # 656
  constant_a = 4;
  result = ((constant_a / i) <= i);
  uni_klee_add_patch(patch_results, 656, result);
  // Patch 72-15 # 657
  constant_a = 5;
  result = ((constant_a / i) <= i);
  uni_klee_add_patch(patch_results, 657, result);
  // Patch 72-16 # 658
  constant_a = 6;
  result = ((constant_a / i) <= i);
  uni_klee_add_patch(patch_results, 658, result);
  // Patch 72-17 # 659
  constant_a = 7;
  result = ((constant_a / i) <= i);
  uni_klee_add_patch(patch_results, 659, result);
  // Patch 72-18 # 660
  constant_a = 8;
  result = ((constant_a / i) <= i);
  uni_klee_add_patch(patch_results, 660, result);
  // Patch 72-19 # 661
  constant_a = 9;
  result = ((constant_a / i) <= i);
  uni_klee_add_patch(patch_results, 661, result);
  // Patch 72-20 # 662
  constant_a = 10;
  result = ((constant_a / i) <= i);
  uni_klee_add_patch(patch_results, 662, result);
  // Patch 73-0 # 663
  constant_a = -10;
  result = ((i / constant_a) < size);
  uni_klee_add_patch(patch_results, 663, result);
  // Patch 73-1 # 664
  constant_a = -9;
  result = ((i / constant_a) < size);
  uni_klee_add_patch(patch_results, 664, result);
  // Patch 73-2 # 665
  constant_a = -8;
  result = ((i / constant_a) < size);
  uni_klee_add_patch(patch_results, 665, result);
  // Patch 73-3 # 666
  constant_a = -7;
  result = ((i / constant_a) < size);
  uni_klee_add_patch(patch_results, 666, result);
  // Patch 73-4 # 667
  constant_a = -6;
  result = ((i / constant_a) < size);
  uni_klee_add_patch(patch_results, 667, result);
  // Patch 73-5 # 668
  constant_a = -5;
  result = ((i / constant_a) < size);
  uni_klee_add_patch(patch_results, 668, result);
  // Patch 73-6 # 669
  constant_a = -4;
  result = ((i / constant_a) < size);
  uni_klee_add_patch(patch_results, 669, result);
  // Patch 73-7 # 670
  constant_a = -3;
  result = ((i / constant_a) < size);
  uni_klee_add_patch(patch_results, 670, result);
  // Patch 73-8 # 671
  constant_a = -2;
  result = ((i / constant_a) < size);
  uni_klee_add_patch(patch_results, 671, result);
  // Patch 73-9 # 672
  constant_a = -1;
  result = ((i / constant_a) < size);
  uni_klee_add_patch(patch_results, 672, result);
  // Patch 73-10 # 673
  constant_a = 1;
  result = ((i / constant_a) < size);
  uni_klee_add_patch(patch_results, 673, result);
  // Patch 73-11 # 674
  constant_a = 2;
  result = ((i / constant_a) < size);
  uni_klee_add_patch(patch_results, 674, result);
  // Patch 73-12 # 675
  constant_a = 3;
  result = ((i / constant_a) < size);
  uni_klee_add_patch(patch_results, 675, result);
  // Patch 73-13 # 676
  constant_a = 4;
  result = ((i / constant_a) < size);
  uni_klee_add_patch(patch_results, 676, result);
  // Patch 73-14 # 677
  constant_a = 5;
  result = ((i / constant_a) < size);
  uni_klee_add_patch(patch_results, 677, result);
  // Patch 73-15 # 678
  constant_a = 6;
  result = ((i / constant_a) < size);
  uni_klee_add_patch(patch_results, 678, result);
  // Patch 73-16 # 679
  constant_a = 7;
  result = ((i / constant_a) < size);
  uni_klee_add_patch(patch_results, 679, result);
  // Patch 73-17 # 680
  constant_a = 8;
  result = ((i / constant_a) < size);
  uni_klee_add_patch(patch_results, 680, result);
  // Patch 73-18 # 681
  constant_a = 9;
  result = ((i / constant_a) < size);
  uni_klee_add_patch(patch_results, 681, result);
  // Patch 73-19 # 682
  constant_a = 10;
  result = ((i / constant_a) < size);
  uni_klee_add_patch(patch_results, 682, result);
  // Patch 74-0 # 683
  constant_a = -10;
  result = ((constant_a / i) < size);
  uni_klee_add_patch(patch_results, 683, result);
  // Patch 74-1 # 684
  constant_a = -9;
  result = ((constant_a / i) < size);
  uni_klee_add_patch(patch_results, 684, result);
  // Patch 74-2 # 685
  constant_a = -8;
  result = ((constant_a / i) < size);
  uni_klee_add_patch(patch_results, 685, result);
  // Patch 74-3 # 686
  constant_a = -7;
  result = ((constant_a / i) < size);
  uni_klee_add_patch(patch_results, 686, result);
  // Patch 74-4 # 687
  constant_a = -6;
  result = ((constant_a / i) < size);
  uni_klee_add_patch(patch_results, 687, result);
  // Patch 74-5 # 688
  constant_a = -5;
  result = ((constant_a / i) < size);
  uni_klee_add_patch(patch_results, 688, result);
  // Patch 74-6 # 689
  constant_a = -4;
  result = ((constant_a / i) < size);
  uni_klee_add_patch(patch_results, 689, result);
  // Patch 74-7 # 690
  constant_a = -3;
  result = ((constant_a / i) < size);
  uni_klee_add_patch(patch_results, 690, result);
  // Patch 74-8 # 691
  constant_a = -2;
  result = ((constant_a / i) < size);
  uni_klee_add_patch(patch_results, 691, result);
  // Patch 74-9 # 692
  constant_a = -1;
  result = ((constant_a / i) < size);
  uni_klee_add_patch(patch_results, 692, result);
  // Patch 74-10 # 693
  constant_a = 0;
  result = ((constant_a / i) < size);
  uni_klee_add_patch(patch_results, 693, result);
  // Patch 74-11 # 694
  constant_a = 1;
  result = ((constant_a / i) < size);
  uni_klee_add_patch(patch_results, 694, result);
  // Patch 74-12 # 695
  constant_a = 2;
  result = ((constant_a / i) < size);
  uni_klee_add_patch(patch_results, 695, result);
  // Patch 74-13 # 696
  constant_a = 3;
  result = ((constant_a / i) < size);
  uni_klee_add_patch(patch_results, 696, result);
  // Patch 74-14 # 697
  constant_a = 4;
  result = ((constant_a / i) < size);
  uni_klee_add_patch(patch_results, 697, result);
  // Patch 74-15 # 698
  constant_a = 5;
  result = ((constant_a / i) < size);
  uni_klee_add_patch(patch_results, 698, result);
  // Patch 74-16 # 699
  constant_a = 6;
  result = ((constant_a / i) < size);
  uni_klee_add_patch(patch_results, 699, result);
  // Patch 74-17 # 700
  constant_a = 7;
  result = ((constant_a / i) < size);
  uni_klee_add_patch(patch_results, 700, result);
  // Patch 74-18 # 701
  constant_a = 8;
  result = ((constant_a / i) < size);
  uni_klee_add_patch(patch_results, 701, result);
  // Patch 74-19 # 702
  constant_a = 9;
  result = ((constant_a / i) < size);
  uni_klee_add_patch(patch_results, 702, result);
  // Patch 74-20 # 703
  constant_a = 10;
  result = ((constant_a / i) < size);
  uni_klee_add_patch(patch_results, 703, result);
  // Patch 75-0 # 704
  constant_a = -10;
  result = ((constant_a / size) < i);
  uni_klee_add_patch(patch_results, 704, result);
  // Patch 75-1 # 705
  constant_a = -9;
  result = ((constant_a / size) < i);
  uni_klee_add_patch(patch_results, 705, result);
  // Patch 75-2 # 706
  constant_a = -8;
  result = ((constant_a / size) < i);
  uni_klee_add_patch(patch_results, 706, result);
  // Patch 75-3 # 707
  constant_a = -7;
  result = ((constant_a / size) < i);
  uni_klee_add_patch(patch_results, 707, result);
  // Patch 75-4 # 708
  constant_a = -6;
  result = ((constant_a / size) < i);
  uni_klee_add_patch(patch_results, 708, result);
  // Patch 75-5 # 709
  constant_a = -5;
  result = ((constant_a / size) < i);
  uni_klee_add_patch(patch_results, 709, result);
  // Patch 75-6 # 710
  constant_a = -4;
  result = ((constant_a / size) < i);
  uni_klee_add_patch(patch_results, 710, result);
  // Patch 75-7 # 711
  constant_a = -3;
  result = ((constant_a / size) < i);
  uni_klee_add_patch(patch_results, 711, result);
  // Patch 75-8 # 712
  constant_a = -2;
  result = ((constant_a / size) < i);
  uni_klee_add_patch(patch_results, 712, result);
  // Patch 75-9 # 713
  constant_a = -1;
  result = ((constant_a / size) < i);
  uni_klee_add_patch(patch_results, 713, result);
  // Patch 75-10 # 714
  constant_a = 0;
  result = ((constant_a / size) < i);
  uni_klee_add_patch(patch_results, 714, result);
  // Patch 75-11 # 715
  constant_a = 1;
  result = ((constant_a / size) < i);
  uni_klee_add_patch(patch_results, 715, result);
  // Patch 75-12 # 716
  constant_a = 2;
  result = ((constant_a / size) < i);
  uni_klee_add_patch(patch_results, 716, result);
  // Patch 75-13 # 717
  constant_a = 3;
  result = ((constant_a / size) < i);
  uni_klee_add_patch(patch_results, 717, result);
  // Patch 75-14 # 718
  constant_a = 4;
  result = ((constant_a / size) < i);
  uni_klee_add_patch(patch_results, 718, result);
  // Patch 75-15 # 719
  constant_a = 5;
  result = ((constant_a / size) < i);
  uni_klee_add_patch(patch_results, 719, result);
  // Patch 75-16 # 720
  constant_a = 6;
  result = ((constant_a / size) < i);
  uni_klee_add_patch(patch_results, 720, result);
  // Patch 75-17 # 721
  constant_a = 7;
  result = ((constant_a / size) < i);
  uni_klee_add_patch(patch_results, 721, result);
  // Patch 75-18 # 722
  constant_a = 8;
  result = ((constant_a / size) < i);
  uni_klee_add_patch(patch_results, 722, result);
  // Patch 75-19 # 723
  constant_a = 9;
  result = ((constant_a / size) < i);
  uni_klee_add_patch(patch_results, 723, result);
  // Patch 75-20 # 724
  constant_a = 10;
  result = ((constant_a / size) < i);
  uni_klee_add_patch(patch_results, 724, result);
  // Patch 76-0 # 725
  constant_a = -10;
  result = ((i / constant_a) != size);
  uni_klee_add_patch(patch_results, 725, result);
  // Patch 76-1 # 726
  constant_a = -9;
  result = ((i / constant_a) != size);
  uni_klee_add_patch(patch_results, 726, result);
  // Patch 76-2 # 727
  constant_a = -8;
  result = ((i / constant_a) != size);
  uni_klee_add_patch(patch_results, 727, result);
  // Patch 76-3 # 728
  constant_a = -7;
  result = ((i / constant_a) != size);
  uni_klee_add_patch(patch_results, 728, result);
  // Patch 76-4 # 729
  constant_a = -6;
  result = ((i / constant_a) != size);
  uni_klee_add_patch(patch_results, 729, result);
  // Patch 76-5 # 730
  constant_a = -5;
  result = ((i / constant_a) != size);
  uni_klee_add_patch(patch_results, 730, result);
  // Patch 76-6 # 731
  constant_a = -4;
  result = ((i / constant_a) != size);
  uni_klee_add_patch(patch_results, 731, result);
  // Patch 76-7 # 732
  constant_a = -3;
  result = ((i / constant_a) != size);
  uni_klee_add_patch(patch_results, 732, result);
  // Patch 76-8 # 733
  constant_a = -2;
  result = ((i / constant_a) != size);
  uni_klee_add_patch(patch_results, 733, result);
  // Patch 76-9 # 734
  constant_a = -1;
  result = ((i / constant_a) != size);
  uni_klee_add_patch(patch_results, 734, result);
  // Patch 76-10 # 735
  constant_a = 1;
  result = ((i / constant_a) != size);
  uni_klee_add_patch(patch_results, 735, result);
  // Patch 76-11 # 736
  constant_a = 2;
  result = ((i / constant_a) != size);
  uni_klee_add_patch(patch_results, 736, result);
  // Patch 76-12 # 737
  constant_a = 3;
  result = ((i / constant_a) != size);
  uni_klee_add_patch(patch_results, 737, result);
  // Patch 76-13 # 738
  constant_a = 4;
  result = ((i / constant_a) != size);
  uni_klee_add_patch(patch_results, 738, result);
  // Patch 76-14 # 739
  constant_a = 5;
  result = ((i / constant_a) != size);
  uni_klee_add_patch(patch_results, 739, result);
  // Patch 76-15 # 740
  constant_a = 6;
  result = ((i / constant_a) != size);
  uni_klee_add_patch(patch_results, 740, result);
  // Patch 76-16 # 741
  constant_a = 7;
  result = ((i / constant_a) != size);
  uni_klee_add_patch(patch_results, 741, result);
  // Patch 76-17 # 742
  constant_a = 8;
  result = ((i / constant_a) != size);
  uni_klee_add_patch(patch_results, 742, result);
  // Patch 76-18 # 743
  constant_a = 9;
  result = ((i / constant_a) != size);
  uni_klee_add_patch(patch_results, 743, result);
  // Patch 76-19 # 744
  constant_a = 10;
  result = ((i / constant_a) != size);
  uni_klee_add_patch(patch_results, 744, result);
  // Patch 77-0 # 745
  constant_a = -10;
  result = ((constant_a / i) != size);
  uni_klee_add_patch(patch_results, 745, result);
  // Patch 77-1 # 746
  constant_a = -9;
  result = ((constant_a / i) != size);
  uni_klee_add_patch(patch_results, 746, result);
  // Patch 77-2 # 747
  constant_a = -8;
  result = ((constant_a / i) != size);
  uni_klee_add_patch(patch_results, 747, result);
  // Patch 77-3 # 748
  constant_a = -7;
  result = ((constant_a / i) != size);
  uni_klee_add_patch(patch_results, 748, result);
  // Patch 77-4 # 749
  constant_a = -6;
  result = ((constant_a / i) != size);
  uni_klee_add_patch(patch_results, 749, result);
  // Patch 77-5 # 750
  constant_a = -5;
  result = ((constant_a / i) != size);
  uni_klee_add_patch(patch_results, 750, result);
  // Patch 77-6 # 751
  constant_a = -4;
  result = ((constant_a / i) != size);
  uni_klee_add_patch(patch_results, 751, result);
  // Patch 77-7 # 752
  constant_a = -3;
  result = ((constant_a / i) != size);
  uni_klee_add_patch(patch_results, 752, result);
  // Patch 77-8 # 753
  constant_a = -2;
  result = ((constant_a / i) != size);
  uni_klee_add_patch(patch_results, 753, result);
  // Patch 77-9 # 754
  constant_a = -1;
  result = ((constant_a / i) != size);
  uni_klee_add_patch(patch_results, 754, result);
  // Patch 77-10 # 755
  constant_a = 0;
  result = ((constant_a / i) != size);
  uni_klee_add_patch(patch_results, 755, result);
  // Patch 77-11 # 756
  constant_a = 1;
  result = ((constant_a / i) != size);
  uni_klee_add_patch(patch_results, 756, result);
  // Patch 77-12 # 757
  constant_a = 2;
  result = ((constant_a / i) != size);
  uni_klee_add_patch(patch_results, 757, result);
  // Patch 77-13 # 758
  constant_a = 3;
  result = ((constant_a / i) != size);
  uni_klee_add_patch(patch_results, 758, result);
  // Patch 77-14 # 759
  constant_a = 4;
  result = ((constant_a / i) != size);
  uni_klee_add_patch(patch_results, 759, result);
  // Patch 77-15 # 760
  constant_a = 5;
  result = ((constant_a / i) != size);
  uni_klee_add_patch(patch_results, 760, result);
  // Patch 77-16 # 761
  constant_a = 6;
  result = ((constant_a / i) != size);
  uni_klee_add_patch(patch_results, 761, result);
  // Patch 77-17 # 762
  constant_a = 7;
  result = ((constant_a / i) != size);
  uni_klee_add_patch(patch_results, 762, result);
  // Patch 77-18 # 763
  constant_a = 8;
  result = ((constant_a / i) != size);
  uni_klee_add_patch(patch_results, 763, result);
  // Patch 77-19 # 764
  constant_a = 9;
  result = ((constant_a / i) != size);
  uni_klee_add_patch(patch_results, 764, result);
  // Patch 77-20 # 765
  constant_a = 10;
  result = ((constant_a / i) != size);
  uni_klee_add_patch(patch_results, 765, result);
  // Patch 78-0 # 766
  constant_a = -10;
  result = ((constant_a / size) != i);
  uni_klee_add_patch(patch_results, 766, result);
  // Patch 78-1 # 767
  constant_a = -9;
  result = ((constant_a / size) != i);
  uni_klee_add_patch(patch_results, 767, result);
  // Patch 78-2 # 768
  constant_a = -8;
  result = ((constant_a / size) != i);
  uni_klee_add_patch(patch_results, 768, result);
  // Patch 78-3 # 769
  constant_a = -7;
  result = ((constant_a / size) != i);
  uni_klee_add_patch(patch_results, 769, result);
  // Patch 78-4 # 770
  constant_a = -6;
  result = ((constant_a / size) != i);
  uni_klee_add_patch(patch_results, 770, result);
  // Patch 78-5 # 771
  constant_a = -5;
  result = ((constant_a / size) != i);
  uni_klee_add_patch(patch_results, 771, result);
  // Patch 78-6 # 772
  constant_a = -4;
  result = ((constant_a / size) != i);
  uni_klee_add_patch(patch_results, 772, result);
  // Patch 78-7 # 773
  constant_a = -3;
  result = ((constant_a / size) != i);
  uni_klee_add_patch(patch_results, 773, result);
  // Patch 78-8 # 774
  constant_a = -2;
  result = ((constant_a / size) != i);
  uni_klee_add_patch(patch_results, 774, result);
  // Patch 78-9 # 775
  constant_a = -1;
  result = ((constant_a / size) != i);
  uni_klee_add_patch(patch_results, 775, result);
  // Patch 78-10 # 776
  constant_a = 0;
  result = ((constant_a / size) != i);
  uni_klee_add_patch(patch_results, 776, result);
  // Patch 78-11 # 777
  constant_a = 1;
  result = ((constant_a / size) != i);
  uni_klee_add_patch(patch_results, 777, result);
  // Patch 78-12 # 778
  constant_a = 2;
  result = ((constant_a / size) != i);
  uni_klee_add_patch(patch_results, 778, result);
  // Patch 78-13 # 779
  constant_a = 3;
  result = ((constant_a / size) != i);
  uni_klee_add_patch(patch_results, 779, result);
  // Patch 78-14 # 780
  constant_a = 4;
  result = ((constant_a / size) != i);
  uni_klee_add_patch(patch_results, 780, result);
  // Patch 78-15 # 781
  constant_a = 5;
  result = ((constant_a / size) != i);
  uni_klee_add_patch(patch_results, 781, result);
  // Patch 78-16 # 782
  constant_a = 6;
  result = ((constant_a / size) != i);
  uni_klee_add_patch(patch_results, 782, result);
  // Patch 78-17 # 783
  constant_a = 7;
  result = ((constant_a / size) != i);
  uni_klee_add_patch(patch_results, 783, result);
  // Patch 78-18 # 784
  constant_a = 8;
  result = ((constant_a / size) != i);
  uni_klee_add_patch(patch_results, 784, result);
  // Patch 78-19 # 785
  constant_a = 9;
  result = ((constant_a / size) != i);
  uni_klee_add_patch(patch_results, 785, result);
  // Patch 78-20 # 786
  constant_a = 10;
  result = ((constant_a / size) != i);
  uni_klee_add_patch(patch_results, 786, result);
  // Patch 79-0 # 787
  constant_a = -10;
  result = ((i / constant_a) <= size);
  uni_klee_add_patch(patch_results, 787, result);
  // Patch 79-1 # 788
  constant_a = -9;
  result = ((i / constant_a) <= size);
  uni_klee_add_patch(patch_results, 788, result);
  // Patch 79-2 # 789
  constant_a = -8;
  result = ((i / constant_a) <= size);
  uni_klee_add_patch(patch_results, 789, result);
  // Patch 79-3 # 790
  constant_a = -7;
  result = ((i / constant_a) <= size);
  uni_klee_add_patch(patch_results, 790, result);
  // Patch 79-4 # 791
  constant_a = -6;
  result = ((i / constant_a) <= size);
  uni_klee_add_patch(patch_results, 791, result);
  // Patch 79-5 # 792
  constant_a = -5;
  result = ((i / constant_a) <= size);
  uni_klee_add_patch(patch_results, 792, result);
  // Patch 79-6 # 793
  constant_a = -4;
  result = ((i / constant_a) <= size);
  uni_klee_add_patch(patch_results, 793, result);
  // Patch 79-7 # 794
  constant_a = -3;
  result = ((i / constant_a) <= size);
  uni_klee_add_patch(patch_results, 794, result);
  // Patch 79-8 # 795
  constant_a = -2;
  result = ((i / constant_a) <= size);
  uni_klee_add_patch(patch_results, 795, result);
  // Patch 79-9 # 796
  constant_a = -1;
  result = ((i / constant_a) <= size);
  uni_klee_add_patch(patch_results, 796, result);
  // Patch 79-10 # 797
  constant_a = 1;
  result = ((i / constant_a) <= size);
  uni_klee_add_patch(patch_results, 797, result);
  // Patch 79-11 # 798
  constant_a = 2;
  result = ((i / constant_a) <= size);
  uni_klee_add_patch(patch_results, 798, result);
  // Patch 79-12 # 799
  constant_a = 3;
  result = ((i / constant_a) <= size);
  uni_klee_add_patch(patch_results, 799, result);
  // Patch 79-13 # 800
  constant_a = 4;
  result = ((i / constant_a) <= size);
  uni_klee_add_patch(patch_results, 800, result);
  // Patch 79-14 # 801
  constant_a = 5;
  result = ((i / constant_a) <= size);
  uni_klee_add_patch(patch_results, 801, result);
  // Patch 79-15 # 802
  constant_a = 6;
  result = ((i / constant_a) <= size);
  uni_klee_add_patch(patch_results, 802, result);
  // Patch 79-16 # 803
  constant_a = 7;
  result = ((i / constant_a) <= size);
  uni_klee_add_patch(patch_results, 803, result);
  // Patch 79-17 # 804
  constant_a = 8;
  result = ((i / constant_a) <= size);
  uni_klee_add_patch(patch_results, 804, result);
  // Patch 79-18 # 805
  constant_a = 9;
  result = ((i / constant_a) <= size);
  uni_klee_add_patch(patch_results, 805, result);
  // Patch 79-19 # 806
  constant_a = 10;
  result = ((i / constant_a) <= size);
  uni_klee_add_patch(patch_results, 806, result);
  // Patch 80-0 # 807
  constant_a = -10;
  result = ((constant_a / i) <= size);
  uni_klee_add_patch(patch_results, 807, result);
  // Patch 80-1 # 808
  constant_a = -9;
  result = ((constant_a / i) <= size);
  uni_klee_add_patch(patch_results, 808, result);
  // Patch 80-2 # 809
  constant_a = -8;
  result = ((constant_a / i) <= size);
  uni_klee_add_patch(patch_results, 809, result);
  // Patch 80-3 # 810
  constant_a = -7;
  result = ((constant_a / i) <= size);
  uni_klee_add_patch(patch_results, 810, result);
  // Patch 80-4 # 811
  constant_a = -6;
  result = ((constant_a / i) <= size);
  uni_klee_add_patch(patch_results, 811, result);
  // Patch 80-5 # 812
  constant_a = -5;
  result = ((constant_a / i) <= size);
  uni_klee_add_patch(patch_results, 812, result);
  // Patch 80-6 # 813
  constant_a = -4;
  result = ((constant_a / i) <= size);
  uni_klee_add_patch(patch_results, 813, result);
  // Patch 80-7 # 814
  constant_a = -3;
  result = ((constant_a / i) <= size);
  uni_klee_add_patch(patch_results, 814, result);
  // Patch 80-8 # 815
  constant_a = -2;
  result = ((constant_a / i) <= size);
  uni_klee_add_patch(patch_results, 815, result);
  // Patch 80-9 # 816
  constant_a = -1;
  result = ((constant_a / i) <= size);
  uni_klee_add_patch(patch_results, 816, result);
  // Patch 80-10 # 817
  constant_a = 0;
  result = ((constant_a / i) <= size);
  uni_klee_add_patch(patch_results, 817, result);
  // Patch 80-11 # 818
  constant_a = 1;
  result = ((constant_a / i) <= size);
  uni_klee_add_patch(patch_results, 818, result);
  // Patch 80-12 # 819
  constant_a = 2;
  result = ((constant_a / i) <= size);
  uni_klee_add_patch(patch_results, 819, result);
  // Patch 80-13 # 820
  constant_a = 3;
  result = ((constant_a / i) <= size);
  uni_klee_add_patch(patch_results, 820, result);
  // Patch 80-14 # 821
  constant_a = 4;
  result = ((constant_a / i) <= size);
  uni_klee_add_patch(patch_results, 821, result);
  // Patch 80-15 # 822
  constant_a = 5;
  result = ((constant_a / i) <= size);
  uni_klee_add_patch(patch_results, 822, result);
  // Patch 80-16 # 823
  constant_a = 6;
  result = ((constant_a / i) <= size);
  uni_klee_add_patch(patch_results, 823, result);
  // Patch 80-17 # 824
  constant_a = 7;
  result = ((constant_a / i) <= size);
  uni_klee_add_patch(patch_results, 824, result);
  // Patch 80-18 # 825
  constant_a = 8;
  result = ((constant_a / i) <= size);
  uni_klee_add_patch(patch_results, 825, result);
  // Patch 80-19 # 826
  constant_a = 9;
  result = ((constant_a / i) <= size);
  uni_klee_add_patch(patch_results, 826, result);
  // Patch 80-20 # 827
  constant_a = 10;
  result = ((constant_a / i) <= size);
  uni_klee_add_patch(patch_results, 827, result);
  // Patch 81-0 # 828
  constant_a = -10;
  result = ((constant_a / size) <= i);
  uni_klee_add_patch(patch_results, 828, result);
  // Patch 81-1 # 829
  constant_a = -9;
  result = ((constant_a / size) <= i);
  uni_klee_add_patch(patch_results, 829, result);
  // Patch 81-2 # 830
  constant_a = -8;
  result = ((constant_a / size) <= i);
  uni_klee_add_patch(patch_results, 830, result);
  // Patch 81-3 # 831
  constant_a = -7;
  result = ((constant_a / size) <= i);
  uni_klee_add_patch(patch_results, 831, result);
  // Patch 81-4 # 832
  constant_a = -6;
  result = ((constant_a / size) <= i);
  uni_klee_add_patch(patch_results, 832, result);
  // Patch 81-5 # 833
  constant_a = -5;
  result = ((constant_a / size) <= i);
  uni_klee_add_patch(patch_results, 833, result);
  // Patch 81-6 # 834
  constant_a = -4;
  result = ((constant_a / size) <= i);
  uni_klee_add_patch(patch_results, 834, result);
  // Patch 81-7 # 835
  constant_a = -3;
  result = ((constant_a / size) <= i);
  uni_klee_add_patch(patch_results, 835, result);
  // Patch 81-8 # 836
  constant_a = -2;
  result = ((constant_a / size) <= i);
  uni_klee_add_patch(patch_results, 836, result);
  // Patch 81-9 # 837
  constant_a = -1;
  result = ((constant_a / size) <= i);
  uni_klee_add_patch(patch_results, 837, result);
  // Patch 81-10 # 838
  constant_a = 0;
  result = ((constant_a / size) <= i);
  uni_klee_add_patch(patch_results, 838, result);
  // Patch 81-11 # 839
  constant_a = 1;
  result = ((constant_a / size) <= i);
  uni_klee_add_patch(patch_results, 839, result);
  // Patch 81-12 # 840
  constant_a = 2;
  result = ((constant_a / size) <= i);
  uni_klee_add_patch(patch_results, 840, result);
  // Patch 81-13 # 841
  constant_a = 3;
  result = ((constant_a / size) <= i);
  uni_klee_add_patch(patch_results, 841, result);
  // Patch 81-14 # 842
  constant_a = 4;
  result = ((constant_a / size) <= i);
  uni_klee_add_patch(patch_results, 842, result);
  // Patch 81-15 # 843
  constant_a = 5;
  result = ((constant_a / size) <= i);
  uni_klee_add_patch(patch_results, 843, result);
  // Patch 81-16 # 844
  constant_a = 6;
  result = ((constant_a / size) <= i);
  uni_klee_add_patch(patch_results, 844, result);
  // Patch 81-17 # 845
  constant_a = 7;
  result = ((constant_a / size) <= i);
  uni_klee_add_patch(patch_results, 845, result);
  // Patch 81-18 # 846
  constant_a = 8;
  result = ((constant_a / size) <= i);
  uni_klee_add_patch(patch_results, 846, result);
  // Patch 81-19 # 847
  constant_a = 9;
  result = ((constant_a / size) <= i);
  uni_klee_add_patch(patch_results, 847, result);
  // Patch 81-20 # 848
  constant_a = 10;
  result = ((constant_a / size) <= i);
  uni_klee_add_patch(patch_results, 848, result);
  // Patch 82-0 # 849
  constant_a = -10;
  result = ((constant_a / size) < size);
  uni_klee_add_patch(patch_results, 849, result);
  // Patch 82-1 # 850
  constant_a = -9;
  result = ((constant_a / size) < size);
  uni_klee_add_patch(patch_results, 850, result);
  // Patch 82-2 # 851
  constant_a = -8;
  result = ((constant_a / size) < size);
  uni_klee_add_patch(patch_results, 851, result);
  // Patch 82-3 # 852
  constant_a = -7;
  result = ((constant_a / size) < size);
  uni_klee_add_patch(patch_results, 852, result);
  // Patch 82-4 # 853
  constant_a = -6;
  result = ((constant_a / size) < size);
  uni_klee_add_patch(patch_results, 853, result);
  // Patch 82-5 # 854
  constant_a = -5;
  result = ((constant_a / size) < size);
  uni_klee_add_patch(patch_results, 854, result);
  // Patch 82-6 # 855
  constant_a = -4;
  result = ((constant_a / size) < size);
  uni_klee_add_patch(patch_results, 855, result);
  // Patch 82-7 # 856
  constant_a = -3;
  result = ((constant_a / size) < size);
  uni_klee_add_patch(patch_results, 856, result);
  // Patch 82-8 # 857
  constant_a = -2;
  result = ((constant_a / size) < size);
  uni_klee_add_patch(patch_results, 857, result);
  // Patch 82-9 # 858
  constant_a = -1;
  result = ((constant_a / size) < size);
  uni_klee_add_patch(patch_results, 858, result);
  // Patch 82-10 # 859
  constant_a = 0;
  result = ((constant_a / size) < size);
  uni_klee_add_patch(patch_results, 859, result);
  // Patch 82-11 # 860
  constant_a = 1;
  result = ((constant_a / size) < size);
  uni_klee_add_patch(patch_results, 860, result);
  // Patch 82-12 # 861
  constant_a = 2;
  result = ((constant_a / size) < size);
  uni_klee_add_patch(patch_results, 861, result);
  // Patch 82-13 # 862
  constant_a = 3;
  result = ((constant_a / size) < size);
  uni_klee_add_patch(patch_results, 862, result);
  // Patch 82-14 # 863
  constant_a = 4;
  result = ((constant_a / size) < size);
  uni_klee_add_patch(patch_results, 863, result);
  // Patch 82-15 # 864
  constant_a = 5;
  result = ((constant_a / size) < size);
  uni_klee_add_patch(patch_results, 864, result);
  // Patch 82-16 # 865
  constant_a = 6;
  result = ((constant_a / size) < size);
  uni_klee_add_patch(patch_results, 865, result);
  // Patch 82-17 # 866
  constant_a = 7;
  result = ((constant_a / size) < size);
  uni_klee_add_patch(patch_results, 866, result);
  // Patch 82-18 # 867
  constant_a = 8;
  result = ((constant_a / size) < size);
  uni_klee_add_patch(patch_results, 867, result);
  // Patch 82-19 # 868
  constant_a = 9;
  result = ((constant_a / size) < size);
  uni_klee_add_patch(patch_results, 868, result);
  // Patch 82-20 # 869
  constant_a = 10;
  result = ((constant_a / size) < size);
  uni_klee_add_patch(patch_results, 869, result);
  // Patch 83-0 # 870
  constant_a = -10;
  result = ((constant_a / size) != size);
  uni_klee_add_patch(patch_results, 870, result);
  // Patch 83-1 # 871
  constant_a = -9;
  result = ((constant_a / size) != size);
  uni_klee_add_patch(patch_results, 871, result);
  // Patch 83-2 # 872
  constant_a = -8;
  result = ((constant_a / size) != size);
  uni_klee_add_patch(patch_results, 872, result);
  // Patch 83-3 # 873
  constant_a = -7;
  result = ((constant_a / size) != size);
  uni_klee_add_patch(patch_results, 873, result);
  // Patch 83-4 # 874
  constant_a = -6;
  result = ((constant_a / size) != size);
  uni_klee_add_patch(patch_results, 874, result);
  // Patch 83-5 # 875
  constant_a = -5;
  result = ((constant_a / size) != size);
  uni_klee_add_patch(patch_results, 875, result);
  // Patch 83-6 # 876
  constant_a = -4;
  result = ((constant_a / size) != size);
  uni_klee_add_patch(patch_results, 876, result);
  // Patch 83-7 # 877
  constant_a = -3;
  result = ((constant_a / size) != size);
  uni_klee_add_patch(patch_results, 877, result);
  // Patch 83-8 # 878
  constant_a = -2;
  result = ((constant_a / size) != size);
  uni_klee_add_patch(patch_results, 878, result);
  // Patch 83-9 # 879
  constant_a = -1;
  result = ((constant_a / size) != size);
  uni_klee_add_patch(patch_results, 879, result);
  // Patch 83-10 # 880
  constant_a = 0;
  result = ((constant_a / size) != size);
  uni_klee_add_patch(patch_results, 880, result);
  // Patch 83-11 # 881
  constant_a = 1;
  result = ((constant_a / size) != size);
  uni_klee_add_patch(patch_results, 881, result);
  // Patch 83-12 # 882
  constant_a = 2;
  result = ((constant_a / size) != size);
  uni_klee_add_patch(patch_results, 882, result);
  // Patch 83-13 # 883
  constant_a = 3;
  result = ((constant_a / size) != size);
  uni_klee_add_patch(patch_results, 883, result);
  // Patch 83-14 # 884
  constant_a = 4;
  result = ((constant_a / size) != size);
  uni_klee_add_patch(patch_results, 884, result);
  // Patch 83-15 # 885
  constant_a = 5;
  result = ((constant_a / size) != size);
  uni_klee_add_patch(patch_results, 885, result);
  // Patch 83-16 # 886
  constant_a = 6;
  result = ((constant_a / size) != size);
  uni_klee_add_patch(patch_results, 886, result);
  // Patch 83-17 # 887
  constant_a = 7;
  result = ((constant_a / size) != size);
  uni_klee_add_patch(patch_results, 887, result);
  // Patch 83-18 # 888
  constant_a = 8;
  result = ((constant_a / size) != size);
  uni_klee_add_patch(patch_results, 888, result);
  // Patch 83-19 # 889
  constant_a = 9;
  result = ((constant_a / size) != size);
  uni_klee_add_patch(patch_results, 889, result);
  // Patch 83-20 # 890
  constant_a = 10;
  result = ((constant_a / size) != size);
  uni_klee_add_patch(patch_results, 890, result);
  // Patch 84-0 # 891
  constant_a = -10;
  result = ((size / constant_a) <= size);
  uni_klee_add_patch(patch_results, 891, result);
  // Patch 84-1 # 892
  constant_a = -9;
  result = ((size / constant_a) <= size);
  uni_klee_add_patch(patch_results, 892, result);
  // Patch 84-2 # 893
  constant_a = -8;
  result = ((size / constant_a) <= size);
  uni_klee_add_patch(patch_results, 893, result);
  // Patch 84-3 # 894
  constant_a = -7;
  result = ((size / constant_a) <= size);
  uni_klee_add_patch(patch_results, 894, result);
  // Patch 84-4 # 895
  constant_a = -6;
  result = ((size / constant_a) <= size);
  uni_klee_add_patch(patch_results, 895, result);
  // Patch 84-5 # 896
  constant_a = -5;
  result = ((size / constant_a) <= size);
  uni_klee_add_patch(patch_results, 896, result);
  // Patch 84-6 # 897
  constant_a = -4;
  result = ((size / constant_a) <= size);
  uni_klee_add_patch(patch_results, 897, result);
  // Patch 84-7 # 898
  constant_a = -3;
  result = ((size / constant_a) <= size);
  uni_klee_add_patch(patch_results, 898, result);
  // Patch 84-8 # 899
  constant_a = -2;
  result = ((size / constant_a) <= size);
  uni_klee_add_patch(patch_results, 899, result);
  // Patch 84-9 # 900
  constant_a = -1;
  result = ((size / constant_a) <= size);
  uni_klee_add_patch(patch_results, 900, result);
  // Patch 84-10 # 901
  constant_a = 1;
  result = ((size / constant_a) <= size);
  uni_klee_add_patch(patch_results, 901, result);
  // Patch 84-11 # 902
  constant_a = 2;
  result = ((size / constant_a) <= size);
  uni_klee_add_patch(patch_results, 902, result);
  // Patch 84-12 # 903
  constant_a = 3;
  result = ((size / constant_a) <= size);
  uni_klee_add_patch(patch_results, 903, result);
  // Patch 84-13 # 904
  constant_a = 4;
  result = ((size / constant_a) <= size);
  uni_klee_add_patch(patch_results, 904, result);
  // Patch 84-14 # 905
  constant_a = 5;
  result = ((size / constant_a) <= size);
  uni_klee_add_patch(patch_results, 905, result);
  // Patch 84-15 # 906
  constant_a = 6;
  result = ((size / constant_a) <= size);
  uni_klee_add_patch(patch_results, 906, result);
  // Patch 84-16 # 907
  constant_a = 7;
  result = ((size / constant_a) <= size);
  uni_klee_add_patch(patch_results, 907, result);
  // Patch 84-17 # 908
  constant_a = 8;
  result = ((size / constant_a) <= size);
  uni_klee_add_patch(patch_results, 908, result);
  // Patch 84-18 # 909
  constant_a = 9;
  result = ((size / constant_a) <= size);
  uni_klee_add_patch(patch_results, 909, result);
  // Patch 84-19 # 910
  constant_a = 10;
  result = ((size / constant_a) <= size);
  uni_klee_add_patch(patch_results, 910, result);
  // Patch 85-0 # 911
  constant_a = -10;
  result = ((constant_a / size) <= size);
  uni_klee_add_patch(patch_results, 911, result);
  // Patch 85-1 # 912
  constant_a = -9;
  result = ((constant_a / size) <= size);
  uni_klee_add_patch(patch_results, 912, result);
  // Patch 85-2 # 913
  constant_a = -8;
  result = ((constant_a / size) <= size);
  uni_klee_add_patch(patch_results, 913, result);
  // Patch 85-3 # 914
  constant_a = -7;
  result = ((constant_a / size) <= size);
  uni_klee_add_patch(patch_results, 914, result);
  // Patch 85-4 # 915
  constant_a = -6;
  result = ((constant_a / size) <= size);
  uni_klee_add_patch(patch_results, 915, result);
  // Patch 85-5 # 916
  constant_a = -5;
  result = ((constant_a / size) <= size);
  uni_klee_add_patch(patch_results, 916, result);
  // Patch 85-6 # 917
  constant_a = -4;
  result = ((constant_a / size) <= size);
  uni_klee_add_patch(patch_results, 917, result);
  // Patch 85-7 # 918
  constant_a = -3;
  result = ((constant_a / size) <= size);
  uni_klee_add_patch(patch_results, 918, result);
  // Patch 85-8 # 919
  constant_a = -2;
  result = ((constant_a / size) <= size);
  uni_klee_add_patch(patch_results, 919, result);
  // Patch 85-9 # 920
  constant_a = -1;
  result = ((constant_a / size) <= size);
  uni_klee_add_patch(patch_results, 920, result);
  // Patch 85-10 # 921
  constant_a = 0;
  result = ((constant_a / size) <= size);
  uni_klee_add_patch(patch_results, 921, result);
  // Patch 85-11 # 922
  constant_a = 1;
  result = ((constant_a / size) <= size);
  uni_klee_add_patch(patch_results, 922, result);
  // Patch 85-12 # 923
  constant_a = 2;
  result = ((constant_a / size) <= size);
  uni_klee_add_patch(patch_results, 923, result);
  // Patch 85-13 # 924
  constant_a = 3;
  result = ((constant_a / size) <= size);
  uni_klee_add_patch(patch_results, 924, result);
  // Patch 85-14 # 925
  constant_a = 4;
  result = ((constant_a / size) <= size);
  uni_klee_add_patch(patch_results, 925, result);
  // Patch 85-15 # 926
  constant_a = 5;
  result = ((constant_a / size) <= size);
  uni_klee_add_patch(patch_results, 926, result);
  // Patch 85-16 # 927
  constant_a = 6;
  result = ((constant_a / size) <= size);
  uni_klee_add_patch(patch_results, 927, result);
  // Patch 85-17 # 928
  constant_a = 7;
  result = ((constant_a / size) <= size);
  uni_klee_add_patch(patch_results, 928, result);
  // Patch 85-18 # 929
  constant_a = 8;
  result = ((constant_a / size) <= size);
  uni_klee_add_patch(patch_results, 929, result);
  // Patch 85-19 # 930
  constant_a = 9;
  result = ((constant_a / size) <= size);
  uni_klee_add_patch(patch_results, 930, result);
  // Patch 85-20 # 931
  constant_a = 10;
  result = ((constant_a / size) <= size);
  uni_klee_add_patch(patch_results, 931, result);
  // Patch correct # 932
  result = (size / 2 >= i);
  uni_klee_add_patch(patch_results, 932, result);
  klee_select_patch(&uni_klee_patch_id);
  return uni_klee_choice(patch_results, uni_klee_patch_id);
}
// UNI_KLEE_END

int __cpr_output(char* id, char* typestr, int value){
  return value;
}
