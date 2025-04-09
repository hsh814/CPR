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
  long long start = rvals[0];
  long long initial_read = rvals[1];
  long long bufsize = rvals[2];
  long long constant_a;
  int patch_results[174];
  // Patch buggy # 0
  result = (initial_read != 18446744073709551615UL || start < initial_read);
  uni_klee_add_patch(patch_results, 0, result);
  // Patch 1-0 # 1
  constant_a = 0;
  result = (constant_a != initial_read);
  uni_klee_add_patch(patch_results, 1, result);
  // Patch 2-0 # 2
  result = (start != start);
  uni_klee_add_patch(patch_results, 2, result);
  // Patch 3-0 # 3
  result = (bufsize == start);
  uni_klee_add_patch(patch_results, 3, result);
  // Patch 4-0 # 4
  result = (bufsize < start);
  uni_klee_add_patch(patch_results, 4, result);
  // Patch 5-0 # 5
  result = (bufsize <= start);
  uni_klee_add_patch(patch_results, 5, result);
  // Patch 6-0 # 6
  result = (start < initial_read);
  uni_klee_add_patch(patch_results, 6, result);
  // Patch 7-0 # 7
  result = (bufsize == initial_read);
  uni_klee_add_patch(patch_results, 7, result);
  // Patch 8-0 # 8
  result = (bufsize < initial_read);
  uni_klee_add_patch(patch_results, 8, result);
  // Patch 9-0 # 9
  result = (bufsize <= initial_read);
  uni_klee_add_patch(patch_results, 9, result);
  // Patch 10-0 # 10
  constant_a = 9;
  result = (constant_a <= start);
  uni_klee_add_patch(patch_results, 10, result);
  // Patch 10-1 # 11
  constant_a = 10;
  result = (constant_a <= start);
  uni_klee_add_patch(patch_results, 11, result);
  // Patch 11-0 # 12
  constant_a = 8;
  result = (constant_a < start);
  uni_klee_add_patch(patch_results, 12, result);
  // Patch 11-1 # 13
  constant_a = 9;
  result = (constant_a < start);
  uni_klee_add_patch(patch_results, 13, result);
  // Patch 11-2 # 14
  constant_a = 10;
  result = (constant_a < start);
  uni_klee_add_patch(patch_results, 14, result);
  // Patch 12-0 # 15
  constant_a = -10;
  result = (start <= constant_a);
  uni_klee_add_patch(patch_results, 15, result);
  // Patch 12-1 # 16
  constant_a = -9;
  result = (start <= constant_a);
  uni_klee_add_patch(patch_results, 16, result);
  // Patch 12-2 # 17
  constant_a = -8;
  result = (start <= constant_a);
  uni_klee_add_patch(patch_results, 17, result);
  // Patch 12-3 # 18
  constant_a = -7;
  result = (start <= constant_a);
  uni_klee_add_patch(patch_results, 18, result);
  // Patch 12-4 # 19
  constant_a = -6;
  result = (start <= constant_a);
  uni_klee_add_patch(patch_results, 19, result);
  // Patch 12-5 # 20
  constant_a = -5;
  result = (start <= constant_a);
  uni_klee_add_patch(patch_results, 20, result);
  // Patch 12-6 # 21
  constant_a = -4;
  result = (start <= constant_a);
  uni_klee_add_patch(patch_results, 21, result);
  // Patch 12-7 # 22
  constant_a = -3;
  result = (start <= constant_a);
  uni_klee_add_patch(patch_results, 22, result);
  // Patch 12-8 # 23
  constant_a = -2;
  result = (start <= constant_a);
  uni_klee_add_patch(patch_results, 23, result);
  // Patch 12-9 # 24
  constant_a = -1;
  result = (start <= constant_a);
  uni_klee_add_patch(patch_results, 24, result);
  // Patch 13-0 # 25
  constant_a = 1;
  result = (constant_a <= initial_read);
  uni_klee_add_patch(patch_results, 25, result);
  // Patch 13-1 # 26
  constant_a = 2;
  result = (constant_a <= initial_read);
  uni_klee_add_patch(patch_results, 26, result);
  // Patch 13-2 # 27
  constant_a = 3;
  result = (constant_a <= initial_read);
  uni_klee_add_patch(patch_results, 27, result);
  // Patch 13-3 # 28
  constant_a = 4;
  result = (constant_a <= initial_read);
  uni_klee_add_patch(patch_results, 28, result);
  // Patch 13-4 # 29
  constant_a = 5;
  result = (constant_a <= initial_read);
  uni_klee_add_patch(patch_results, 29, result);
  // Patch 13-5 # 30
  constant_a = 6;
  result = (constant_a <= initial_read);
  uni_klee_add_patch(patch_results, 30, result);
  // Patch 13-6 # 31
  constant_a = 7;
  result = (constant_a <= initial_read);
  uni_klee_add_patch(patch_results, 31, result);
  // Patch 13-7 # 32
  constant_a = 8;
  result = (constant_a <= initial_read);
  uni_klee_add_patch(patch_results, 32, result);
  // Patch 13-8 # 33
  constant_a = 9;
  result = (constant_a <= initial_read);
  uni_klee_add_patch(patch_results, 33, result);
  // Patch 13-9 # 34
  constant_a = 10;
  result = (constant_a <= initial_read);
  uni_klee_add_patch(patch_results, 34, result);
  // Patch 14-0 # 35
  constant_a = -10;
  result = (initial_read <= constant_a);
  uni_klee_add_patch(patch_results, 35, result);
  // Patch 14-1 # 36
  constant_a = -9;
  result = (initial_read <= constant_a);
  uni_klee_add_patch(patch_results, 36, result);
  // Patch 14-2 # 37
  constant_a = -8;
  result = (initial_read <= constant_a);
  uni_klee_add_patch(patch_results, 37, result);
  // Patch 14-3 # 38
  constant_a = -7;
  result = (initial_read <= constant_a);
  uni_klee_add_patch(patch_results, 38, result);
  // Patch 14-4 # 39
  constant_a = -6;
  result = (initial_read <= constant_a);
  uni_klee_add_patch(patch_results, 39, result);
  // Patch 14-5 # 40
  constant_a = -5;
  result = (initial_read <= constant_a);
  uni_klee_add_patch(patch_results, 40, result);
  // Patch 14-6 # 41
  constant_a = -4;
  result = (initial_read <= constant_a);
  uni_klee_add_patch(patch_results, 41, result);
  // Patch 14-7 # 42
  constant_a = -3;
  result = (initial_read <= constant_a);
  uni_klee_add_patch(patch_results, 42, result);
  // Patch 14-8 # 43
  constant_a = -2;
  result = (initial_read <= constant_a);
  uni_klee_add_patch(patch_results, 43, result);
  // Patch 14-9 # 44
  constant_a = -1;
  result = (initial_read <= constant_a);
  uni_klee_add_patch(patch_results, 44, result);
  // Patch 15-0 # 45
  constant_a = -10;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 45, result);
  // Patch 15-1 # 46
  constant_a = -9;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 46, result);
  // Patch 15-2 # 47
  constant_a = -8;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 47, result);
  // Patch 15-3 # 48
  constant_a = -7;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 48, result);
  // Patch 15-4 # 49
  constant_a = -6;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 49, result);
  // Patch 15-5 # 50
  constant_a = -5;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 50, result);
  // Patch 15-6 # 51
  constant_a = -4;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 51, result);
  // Patch 15-7 # 52
  constant_a = -3;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 52, result);
  // Patch 15-8 # 53
  constant_a = -2;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 53, result);
  // Patch 15-9 # 54
  constant_a = -1;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 54, result);
  // Patch 15-10 # 55
  constant_a = 0;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 55, result);
  // Patch 16-0 # 56
  constant_a = 0;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 56, result);
  // Patch 16-1 # 57
  constant_a = 1;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 57, result);
  // Patch 16-2 # 58
  constant_a = 2;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 58, result);
  // Patch 16-3 # 59
  constant_a = 3;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 59, result);
  // Patch 16-4 # 60
  constant_a = 4;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 60, result);
  // Patch 16-5 # 61
  constant_a = 5;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 61, result);
  // Patch 16-6 # 62
  constant_a = 6;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 62, result);
  // Patch 16-7 # 63
  constant_a = 7;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 63, result);
  // Patch 16-8 # 64
  constant_a = 8;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 64, result);
  // Patch 16-9 # 65
  constant_a = 9;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 65, result);
  // Patch 16-10 # 66
  constant_a = 10;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 66, result);
  // Patch 17-0 # 67
  constant_a = -10;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 67, result);
  // Patch 17-1 # 68
  constant_a = -9;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 68, result);
  // Patch 17-2 # 69
  constant_a = -8;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 69, result);
  // Patch 17-3 # 70
  constant_a = -7;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 70, result);
  // Patch 17-4 # 71
  constant_a = -6;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 71, result);
  // Patch 17-5 # 72
  constant_a = -5;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 72, result);
  // Patch 17-6 # 73
  constant_a = -4;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 73, result);
  // Patch 17-7 # 74
  constant_a = -3;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 74, result);
  // Patch 17-8 # 75
  constant_a = -2;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 75, result);
  // Patch 17-9 # 76
  constant_a = -1;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 76, result);
  // Patch 17-10 # 77
  constant_a = 0;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 77, result);
  // Patch 18-0 # 78
  constant_a = -10;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 78, result);
  // Patch 18-1 # 79
  constant_a = -9;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 79, result);
  // Patch 18-2 # 80
  constant_a = -8;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 80, result);
  // Patch 18-3 # 81
  constant_a = -7;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 81, result);
  // Patch 18-4 # 82
  constant_a = -6;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 82, result);
  // Patch 18-5 # 83
  constant_a = -5;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 83, result);
  // Patch 18-6 # 84
  constant_a = -4;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 84, result);
  // Patch 18-7 # 85
  constant_a = -3;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 85, result);
  // Patch 18-8 # 86
  constant_a = -2;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 86, result);
  // Patch 18-9 # 87
  constant_a = -1;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 87, result);
  // Patch 18-10 # 88
  constant_a = 9;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 88, result);
  // Patch 18-11 # 89
  constant_a = 10;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 89, result);
  // Patch 19-0 # 90
  constant_a = -10;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 90, result);
  // Patch 19-1 # 91
  constant_a = -9;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 91, result);
  // Patch 19-2 # 92
  constant_a = -8;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 92, result);
  // Patch 19-3 # 93
  constant_a = -7;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 93, result);
  // Patch 19-4 # 94
  constant_a = -6;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 94, result);
  // Patch 19-5 # 95
  constant_a = -5;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 95, result);
  // Patch 19-6 # 96
  constant_a = -4;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 96, result);
  // Patch 19-7 # 97
  constant_a = -3;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 97, result);
  // Patch 19-8 # 98
  constant_a = -2;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 98, result);
  // Patch 19-9 # 99
  constant_a = -1;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 99, result);
  // Patch 19-10 # 100
  constant_a = 1;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 100, result);
  // Patch 19-11 # 101
  constant_a = 2;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 101, result);
  // Patch 19-12 # 102
  constant_a = 3;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 102, result);
  // Patch 19-13 # 103
  constant_a = 4;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 103, result);
  // Patch 19-14 # 104
  constant_a = 5;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 104, result);
  // Patch 19-15 # 105
  constant_a = 6;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 105, result);
  // Patch 19-16 # 106
  constant_a = 7;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 106, result);
  // Patch 19-17 # 107
  constant_a = 8;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 107, result);
  // Patch 19-18 # 108
  constant_a = 9;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 108, result);
  // Patch 19-19 # 109
  constant_a = 10;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 109, result);
  // Patch 20-0 # 110
  constant_a = -10;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 110, result);
  // Patch 20-1 # 111
  constant_a = -9;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 111, result);
  // Patch 20-2 # 112
  constant_a = -8;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 112, result);
  // Patch 20-3 # 113
  constant_a = -7;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 113, result);
  // Patch 20-4 # 114
  constant_a = -6;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 114, result);
  // Patch 20-5 # 115
  constant_a = -5;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 115, result);
  // Patch 20-6 # 116
  constant_a = -4;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 116, result);
  // Patch 20-7 # 117
  constant_a = -3;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 117, result);
  // Patch 20-8 # 118
  constant_a = -2;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 118, result);
  // Patch 20-9 # 119
  constant_a = -1;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 119, result);
  // Patch 20-10 # 120
  constant_a = 0;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 120, result);
  // Patch 20-11 # 121
  constant_a = 1;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 121, result);
  // Patch 20-12 # 122
  constant_a = 2;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 122, result);
  // Patch 20-13 # 123
  constant_a = 3;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 123, result);
  // Patch 20-14 # 124
  constant_a = 4;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 124, result);
  // Patch 20-15 # 125
  constant_a = 5;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 125, result);
  // Patch 20-16 # 126
  constant_a = 6;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 126, result);
  // Patch 20-17 # 127
  constant_a = 7;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 127, result);
  // Patch 20-18 # 128
  constant_a = 8;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 128, result);
  // Patch 20-19 # 129
  constant_a = 9;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 129, result);
  // Patch 20-20 # 130
  constant_a = 10;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 130, result);
  // Patch 21-0 # 131
  constant_a = -10;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 131, result);
  // Patch 21-1 # 132
  constant_a = -9;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 132, result);
  // Patch 21-2 # 133
  constant_a = -8;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 133, result);
  // Patch 21-3 # 134
  constant_a = -7;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 134, result);
  // Patch 21-4 # 135
  constant_a = -6;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 135, result);
  // Patch 21-5 # 136
  constant_a = -5;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 136, result);
  // Patch 21-6 # 137
  constant_a = -4;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 137, result);
  // Patch 21-7 # 138
  constant_a = -3;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 138, result);
  // Patch 21-8 # 139
  constant_a = -2;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 139, result);
  // Patch 21-9 # 140
  constant_a = -1;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 140, result);
  // Patch 21-10 # 141
  constant_a = 0;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 141, result);
  // Patch 21-11 # 142
  constant_a = 1;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 142, result);
  // Patch 21-12 # 143
  constant_a = 2;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 143, result);
  // Patch 21-13 # 144
  constant_a = 3;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 144, result);
  // Patch 21-14 # 145
  constant_a = 4;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 145, result);
  // Patch 21-15 # 146
  constant_a = 5;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 146, result);
  // Patch 21-16 # 147
  constant_a = 6;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 147, result);
  // Patch 21-17 # 148
  constant_a = 7;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 148, result);
  // Patch 21-18 # 149
  constant_a = 8;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 149, result);
  // Patch 21-19 # 150
  constant_a = 9;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 150, result);
  // Patch 21-20 # 151
  constant_a = 10;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 151, result);
  // Patch 22-0 # 152
  constant_a = -10;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 152, result);
  // Patch 22-1 # 153
  constant_a = -9;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 153, result);
  // Patch 22-2 # 154
  constant_a = -8;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 154, result);
  // Patch 22-3 # 155
  constant_a = -7;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 155, result);
  // Patch 22-4 # 156
  constant_a = -6;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 156, result);
  // Patch 22-5 # 157
  constant_a = -5;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 157, result);
  // Patch 22-6 # 158
  constant_a = -4;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 158, result);
  // Patch 22-7 # 159
  constant_a = -3;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 159, result);
  // Patch 22-8 # 160
  constant_a = -2;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 160, result);
  // Patch 22-9 # 161
  constant_a = -1;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 161, result);
  // Patch 22-10 # 162
  constant_a = 0;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 162, result);
  // Patch 22-11 # 163
  constant_a = 1;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 163, result);
  // Patch 22-12 # 164
  constant_a = 2;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 164, result);
  // Patch 22-13 # 165
  constant_a = 3;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 165, result);
  // Patch 22-14 # 166
  constant_a = 4;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 166, result);
  // Patch 22-15 # 167
  constant_a = 5;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 167, result);
  // Patch 22-16 # 168
  constant_a = 6;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 168, result);
  // Patch 22-17 # 169
  constant_a = 7;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 169, result);
  // Patch 22-18 # 170
  constant_a = 8;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 170, result);
  // Patch 22-19 # 171
  constant_a = 9;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 171, result);
  // Patch 22-20 # 172
  constant_a = 10;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 172, result);
  // Patch correct # 173
  result = (start < initial_read);
  uni_klee_add_patch(patch_results, 173, result);
  klee_select_patch(&uni_klee_patch_id);
  return uni_klee_choice(patch_results, uni_klee_patch_id);
}
// UNI_KLEE_END

int __cpr_output(char* id, char* typestr, int value){
  return value;
}
