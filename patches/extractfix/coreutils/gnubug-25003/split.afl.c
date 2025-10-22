/* split.c -- split a file into pieces.
   Copyright (C) 1988-2016 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* By tege@sics.se, with rms.

   TODO:
   * support -p REGEX as in BSD's split.
   * support --suppress-matched as in csplit.  */
#include <config.h>

#include <assert.h>
#include <stdio.h>
#include <getopt.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "system.h"
#include "die.h"
#include "error.h"
#include "fd-reopen.h"
#include "fcntl--.h"
#include "full-write.h"
#include "ioblksize.h"
#include "quote.h"
#include "safe-read.h"
#include "sig2str.h"
#include "xfreopen.h"
#include "xdectoint.h"
#include "xstrtol.h"


int uni_klee_patch_id;

void klee_select_patch(int *patch_id) {
  *patch_id = atoi(getenv("DAFL_PATCH_ID"));
}

void uni_klee_add_patch(int *patch_results, int patch_id, int result) {
  patch_results[patch_id] = result;
}

int uni_klee_choice(int *patch_results, int patch_id) {
  // FILE *fp=fopen(getenv("DAFL_RESULT_FILE"),"a");
  // if (fp == NULL) {
  //   fprintf(stderr, "Error opening file!\n");
  //   exit(1);
  // }
  // fprintf(fp, "%d ", patch_results[patch_id]);
  // fclose(fp);
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
  int patch_results[390];
  // Patch buggy # 0
  result = (initial_read != 18446744073709551615UL || start < initial_read);
  uni_klee_add_patch(patch_results, 0, result);
  // Patch 1-0 # 1
  result = (initial_read == start);
  uni_klee_add_patch(patch_results, 1, result);
  // Patch 2-0 # 2
  result = (bufsize == start);
  uni_klee_add_patch(patch_results, 2, result);
  // Patch 3-0 # 3
  constant_a = -10;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 3, result);
  // Patch 3-1 # 4
  constant_a = -9;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 4, result);
  // Patch 3-2 # 5
  constant_a = -8;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 5, result);
  // Patch 3-3 # 6
  constant_a = -7;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 6, result);
  // Patch 3-4 # 7
  constant_a = -6;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 7, result);
  // Patch 3-5 # 8
  constant_a = -5;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 8, result);
  // Patch 3-6 # 9
  constant_a = -4;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 9, result);
  // Patch 3-7 # 10
  constant_a = -3;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 10, result);
  // Patch 3-8 # 11
  constant_a = -2;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 11, result);
  // Patch 3-9 # 12
  constant_a = -1;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 12, result);
  // Patch 3-10 # 13
  constant_a = 0;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 13, result);
  // Patch 3-11 # 14
  constant_a = 1;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 14, result);
  // Patch 3-12 # 15
  constant_a = 2;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 15, result);
  // Patch 3-13 # 16
  constant_a = 3;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 16, result);
  // Patch 3-14 # 17
  constant_a = 4;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 17, result);
  // Patch 3-15 # 18
  constant_a = 5;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 18, result);
  // Patch 3-16 # 19
  constant_a = 6;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 19, result);
  // Patch 3-17 # 20
  constant_a = 7;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 20, result);
  // Patch 3-18 # 21
  constant_a = 8;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 21, result);
  // Patch 3-19 # 22
  constant_a = 9;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 22, result);
  // Patch 3-20 # 23
  constant_a = 10;
  result = (constant_a == start);
  uni_klee_add_patch(patch_results, 23, result);
  // Patch 4-0 # 24
  result = (bufsize == initial_read);
  uni_klee_add_patch(patch_results, 24, result);
  // Patch 5-0 # 25
  constant_a = -10;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 25, result);
  // Patch 5-1 # 26
  constant_a = -9;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 26, result);
  // Patch 5-2 # 27
  constant_a = -8;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 27, result);
  // Patch 5-3 # 28
  constant_a = -7;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 28, result);
  // Patch 5-4 # 29
  constant_a = -6;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 29, result);
  // Patch 5-5 # 30
  constant_a = -5;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 30, result);
  // Patch 5-6 # 31
  constant_a = -4;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 31, result);
  // Patch 5-7 # 32
  constant_a = -3;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 32, result);
  // Patch 5-8 # 33
  constant_a = -2;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 33, result);
  // Patch 5-9 # 34
  constant_a = -1;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 34, result);
  // Patch 5-10 # 35
  constant_a = 0;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 35, result);
  // Patch 5-11 # 36
  constant_a = 1;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 36, result);
  // Patch 5-12 # 37
  constant_a = 2;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 37, result);
  // Patch 5-13 # 38
  constant_a = 3;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 38, result);
  // Patch 5-14 # 39
  constant_a = 4;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 39, result);
  // Patch 5-15 # 40
  constant_a = 5;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 40, result);
  // Patch 5-16 # 41
  constant_a = 6;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 41, result);
  // Patch 5-17 # 42
  constant_a = 7;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 42, result);
  // Patch 5-18 # 43
  constant_a = 8;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 43, result);
  // Patch 5-19 # 44
  constant_a = 9;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 44, result);
  // Patch 5-20 # 45
  constant_a = 10;
  result = (constant_a == initial_read);
  uni_klee_add_patch(patch_results, 45, result);
  // Patch 6-0 # 46
  constant_a = -10;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 46, result);
  // Patch 6-1 # 47
  constant_a = -9;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 47, result);
  // Patch 6-2 # 48
  constant_a = -8;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 48, result);
  // Patch 6-3 # 49
  constant_a = -7;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 49, result);
  // Patch 6-4 # 50
  constant_a = -6;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 50, result);
  // Patch 6-5 # 51
  constant_a = -5;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 51, result);
  // Patch 6-6 # 52
  constant_a = -4;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 52, result);
  // Patch 6-7 # 53
  constant_a = -3;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 53, result);
  // Patch 6-8 # 54
  constant_a = -2;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 54, result);
  // Patch 6-9 # 55
  constant_a = -1;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 55, result);
  // Patch 6-10 # 56
  constant_a = 0;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 56, result);
  // Patch 6-11 # 57
  constant_a = 1;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 57, result);
  // Patch 6-12 # 58
  constant_a = 2;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 58, result);
  // Patch 6-13 # 59
  constant_a = 3;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 59, result);
  // Patch 6-14 # 60
  constant_a = 4;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 60, result);
  // Patch 6-15 # 61
  constant_a = 5;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 61, result);
  // Patch 6-16 # 62
  constant_a = 6;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 62, result);
  // Patch 6-17 # 63
  constant_a = 7;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 63, result);
  // Patch 6-18 # 64
  constant_a = 8;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 64, result);
  // Patch 6-19 # 65
  constant_a = 9;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 65, result);
  // Patch 6-20 # 66
  constant_a = 10;
  result = (constant_a == bufsize);
  uni_klee_add_patch(patch_results, 66, result);
  // Patch 7-0 # 67
  result = (start != start);
  uni_klee_add_patch(patch_results, 67, result);
  // Patch 8-0 # 68
  constant_a = -10;
  result = (constant_a != start);
  uni_klee_add_patch(patch_results, 68, result);
  // Patch 8-1 # 69
  constant_a = -9;
  result = (constant_a != start);
  uni_klee_add_patch(patch_results, 69, result);
  // Patch 8-2 # 70
  constant_a = -8;
  result = (constant_a != start);
  uni_klee_add_patch(patch_results, 70, result);
  // Patch 8-3 # 71
  constant_a = -7;
  result = (constant_a != start);
  uni_klee_add_patch(patch_results, 71, result);
  // Patch 8-4 # 72
  constant_a = -6;
  result = (constant_a != start);
  uni_klee_add_patch(patch_results, 72, result);
  // Patch 8-5 # 73
  constant_a = -5;
  result = (constant_a != start);
  uni_klee_add_patch(patch_results, 73, result);
  // Patch 8-6 # 74
  constant_a = -4;
  result = (constant_a != start);
  uni_klee_add_patch(patch_results, 74, result);
  // Patch 8-7 # 75
  constant_a = -3;
  result = (constant_a != start);
  uni_klee_add_patch(patch_results, 75, result);
  // Patch 8-8 # 76
  constant_a = -2;
  result = (constant_a != start);
  uni_klee_add_patch(patch_results, 76, result);
  // Patch 8-9 # 77
  constant_a = -1;
  result = (constant_a != start);
  uni_klee_add_patch(patch_results, 77, result);
  // Patch 8-10 # 78
  constant_a = 0;
  result = (constant_a != start);
  uni_klee_add_patch(patch_results, 78, result);
  // Patch 8-11 # 79
  constant_a = 1;
  result = (constant_a != start);
  uni_klee_add_patch(patch_results, 79, result);
  // Patch 8-12 # 80
  constant_a = 2;
  result = (constant_a != start);
  uni_klee_add_patch(patch_results, 80, result);
  // Patch 8-13 # 81
  constant_a = 3;
  result = (constant_a != start);
  uni_klee_add_patch(patch_results, 81, result);
  // Patch 8-14 # 82
  constant_a = 4;
  result = (constant_a != start);
  uni_klee_add_patch(patch_results, 82, result);
  // Patch 8-15 # 83
  constant_a = 5;
  result = (constant_a != start);
  uni_klee_add_patch(patch_results, 83, result);
  // Patch 8-16 # 84
  constant_a = 6;
  result = (constant_a != start);
  uni_klee_add_patch(patch_results, 84, result);
  // Patch 8-17 # 85
  constant_a = 7;
  result = (constant_a != start);
  uni_klee_add_patch(patch_results, 85, result);
  // Patch 8-18 # 86
  constant_a = 8;
  result = (constant_a != start);
  uni_klee_add_patch(patch_results, 86, result);
  // Patch 8-19 # 87
  constant_a = 9;
  result = (constant_a != start);
  uni_klee_add_patch(patch_results, 87, result);
  // Patch 8-20 # 88
  constant_a = 10;
  result = (constant_a != start);
  uni_klee_add_patch(patch_results, 88, result);
  // Patch 9-0 # 89
  constant_a = -10;
  result = (constant_a != initial_read);
  uni_klee_add_patch(patch_results, 89, result);
  // Patch 9-1 # 90
  constant_a = -9;
  result = (constant_a != initial_read);
  uni_klee_add_patch(patch_results, 90, result);
  // Patch 9-2 # 91
  constant_a = -8;
  result = (constant_a != initial_read);
  uni_klee_add_patch(patch_results, 91, result);
  // Patch 9-3 # 92
  constant_a = -7;
  result = (constant_a != initial_read);
  uni_klee_add_patch(patch_results, 92, result);
  // Patch 9-4 # 93
  constant_a = -6;
  result = (constant_a != initial_read);
  uni_klee_add_patch(patch_results, 93, result);
  // Patch 9-5 # 94
  constant_a = -5;
  result = (constant_a != initial_read);
  uni_klee_add_patch(patch_results, 94, result);
  // Patch 9-6 # 95
  constant_a = -4;
  result = (constant_a != initial_read);
  uni_klee_add_patch(patch_results, 95, result);
  // Patch 9-7 # 96
  constant_a = -3;
  result = (constant_a != initial_read);
  uni_klee_add_patch(patch_results, 96, result);
  // Patch 9-8 # 97
  constant_a = -2;
  result = (constant_a != initial_read);
  uni_klee_add_patch(patch_results, 97, result);
  // Patch 9-9 # 98
  constant_a = -1;
  result = (constant_a != initial_read);
  uni_klee_add_patch(patch_results, 98, result);
  // Patch 9-10 # 99
  constant_a = 0;
  result = (constant_a != initial_read);
  uni_klee_add_patch(patch_results, 99, result);
  // Patch 9-11 # 100
  constant_a = 1;
  result = (constant_a != initial_read);
  uni_klee_add_patch(patch_results, 100, result);
  // Patch 9-12 # 101
  constant_a = 2;
  result = (constant_a != initial_read);
  uni_klee_add_patch(patch_results, 101, result);
  // Patch 9-13 # 102
  constant_a = 3;
  result = (constant_a != initial_read);
  uni_klee_add_patch(patch_results, 102, result);
  // Patch 9-14 # 103
  constant_a = 4;
  result = (constant_a != initial_read);
  uni_klee_add_patch(patch_results, 103, result);
  // Patch 9-15 # 104
  constant_a = 5;
  result = (constant_a != initial_read);
  uni_klee_add_patch(patch_results, 104, result);
  // Patch 9-16 # 105
  constant_a = 6;
  result = (constant_a != initial_read);
  uni_klee_add_patch(patch_results, 105, result);
  // Patch 9-17 # 106
  constant_a = 7;
  result = (constant_a != initial_read);
  uni_klee_add_patch(patch_results, 106, result);
  // Patch 9-18 # 107
  constant_a = 8;
  result = (constant_a != initial_read);
  uni_klee_add_patch(patch_results, 107, result);
  // Patch 9-19 # 108
  constant_a = 9;
  result = (constant_a != initial_read);
  uni_klee_add_patch(patch_results, 108, result);
  // Patch 9-20 # 109
  constant_a = 10;
  result = (constant_a != initial_read);
  uni_klee_add_patch(patch_results, 109, result);
  // Patch 10-0 # 110
  constant_a = -10;
  result = (constant_a != bufsize);
  uni_klee_add_patch(patch_results, 110, result);
  // Patch 10-1 # 111
  constant_a = -9;
  result = (constant_a != bufsize);
  uni_klee_add_patch(patch_results, 111, result);
  // Patch 10-2 # 112
  constant_a = -8;
  result = (constant_a != bufsize);
  uni_klee_add_patch(patch_results, 112, result);
  // Patch 10-3 # 113
  constant_a = -7;
  result = (constant_a != bufsize);
  uni_klee_add_patch(patch_results, 113, result);
  // Patch 10-4 # 114
  constant_a = -6;
  result = (constant_a != bufsize);
  uni_klee_add_patch(patch_results, 114, result);
  // Patch 10-5 # 115
  constant_a = -5;
  result = (constant_a != bufsize);
  uni_klee_add_patch(patch_results, 115, result);
  // Patch 10-6 # 116
  constant_a = -4;
  result = (constant_a != bufsize);
  uni_klee_add_patch(patch_results, 116, result);
  // Patch 10-7 # 117
  constant_a = -3;
  result = (constant_a != bufsize);
  uni_klee_add_patch(patch_results, 117, result);
  // Patch 10-8 # 118
  constant_a = -2;
  result = (constant_a != bufsize);
  uni_klee_add_patch(patch_results, 118, result);
  // Patch 10-9 # 119
  constant_a = -1;
  result = (constant_a != bufsize);
  uni_klee_add_patch(patch_results, 119, result);
  // Patch 10-10 # 120
  constant_a = 0;
  result = (constant_a != bufsize);
  uni_klee_add_patch(patch_results, 120, result);
  // Patch 10-11 # 121
  constant_a = 1;
  result = (constant_a != bufsize);
  uni_klee_add_patch(patch_results, 121, result);
  // Patch 10-12 # 122
  constant_a = 2;
  result = (constant_a != bufsize);
  uni_klee_add_patch(patch_results, 122, result);
  // Patch 10-13 # 123
  constant_a = 3;
  result = (constant_a != bufsize);
  uni_klee_add_patch(patch_results, 123, result);
  // Patch 10-14 # 124
  constant_a = 4;
  result = (constant_a != bufsize);
  uni_klee_add_patch(patch_results, 124, result);
  // Patch 10-15 # 125
  constant_a = 5;
  result = (constant_a != bufsize);
  uni_klee_add_patch(patch_results, 125, result);
  // Patch 10-16 # 126
  constant_a = 6;
  result = (constant_a != bufsize);
  uni_klee_add_patch(patch_results, 126, result);
  // Patch 10-17 # 127
  constant_a = 7;
  result = (constant_a != bufsize);
  uni_klee_add_patch(patch_results, 127, result);
  // Patch 10-18 # 128
  constant_a = 8;
  result = (constant_a != bufsize);
  uni_klee_add_patch(patch_results, 128, result);
  // Patch 10-19 # 129
  constant_a = 9;
  result = (constant_a != bufsize);
  uni_klee_add_patch(patch_results, 129, result);
  // Patch 10-20 # 130
  constant_a = 10;
  result = (constant_a != bufsize);
  uni_klee_add_patch(patch_results, 130, result);
  // Patch 11-0 # 131
  result = (bufsize < start);
  uni_klee_add_patch(patch_results, 131, result);
  // Patch 12-0 # 132
  constant_a = -10;
  result = (constant_a < start);
  uni_klee_add_patch(patch_results, 132, result);
  // Patch 12-1 # 133
  constant_a = -9;
  result = (constant_a < start);
  uni_klee_add_patch(patch_results, 133, result);
  // Patch 12-2 # 134
  constant_a = -8;
  result = (constant_a < start);
  uni_klee_add_patch(patch_results, 134, result);
  // Patch 12-3 # 135
  constant_a = -7;
  result = (constant_a < start);
  uni_klee_add_patch(patch_results, 135, result);
  // Patch 12-4 # 136
  constant_a = -6;
  result = (constant_a < start);
  uni_klee_add_patch(patch_results, 136, result);
  // Patch 12-5 # 137
  constant_a = -5;
  result = (constant_a < start);
  uni_klee_add_patch(patch_results, 137, result);
  // Patch 12-6 # 138
  constant_a = -4;
  result = (constant_a < start);
  uni_klee_add_patch(patch_results, 138, result);
  // Patch 12-7 # 139
  constant_a = -3;
  result = (constant_a < start);
  uni_klee_add_patch(patch_results, 139, result);
  // Patch 12-8 # 140
  constant_a = -2;
  result = (constant_a < start);
  uni_klee_add_patch(patch_results, 140, result);
  // Patch 12-9 # 141
  constant_a = -1;
  result = (constant_a < start);
  uni_klee_add_patch(patch_results, 141, result);
  // Patch 12-10 # 142
  constant_a = 0;
  result = (constant_a < start);
  uni_klee_add_patch(patch_results, 142, result);
  // Patch 12-11 # 143
  constant_a = 1;
  result = (constant_a < start);
  uni_klee_add_patch(patch_results, 143, result);
  // Patch 12-12 # 144
  constant_a = 2;
  result = (constant_a < start);
  uni_klee_add_patch(patch_results, 144, result);
  // Patch 12-13 # 145
  constant_a = 3;
  result = (constant_a < start);
  uni_klee_add_patch(patch_results, 145, result);
  // Patch 12-14 # 146
  constant_a = 4;
  result = (constant_a < start);
  uni_klee_add_patch(patch_results, 146, result);
  // Patch 12-15 # 147
  constant_a = 5;
  result = (constant_a < start);
  uni_klee_add_patch(patch_results, 147, result);
  // Patch 12-16 # 148
  constant_a = 6;
  result = (constant_a < start);
  uni_klee_add_patch(patch_results, 148, result);
  // Patch 12-17 # 149
  constant_a = 7;
  result = (constant_a < start);
  uni_klee_add_patch(patch_results, 149, result);
  // Patch 12-18 # 150
  constant_a = 8;
  result = (constant_a < start);
  uni_klee_add_patch(patch_results, 150, result);
  // Patch 12-19 # 151
  constant_a = 9;
  result = (constant_a < start);
  uni_klee_add_patch(patch_results, 151, result);
  // Patch 12-20 # 152
  constant_a = 10;
  result = (constant_a < start);
  uni_klee_add_patch(patch_results, 152, result);
  // Patch 13-0 # 153
  result = (start < initial_read);
  uni_klee_add_patch(patch_results, 153, result);
  // Patch 14-0 # 154
  result = (bufsize < initial_read);
  uni_klee_add_patch(patch_results, 154, result);
  // Patch 15-0 # 155
  constant_a = -10;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 155, result);
  // Patch 15-1 # 156
  constant_a = -9;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 156, result);
  // Patch 15-2 # 157
  constant_a = -8;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 157, result);
  // Patch 15-3 # 158
  constant_a = -7;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 158, result);
  // Patch 15-4 # 159
  constant_a = -6;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 159, result);
  // Patch 15-5 # 160
  constant_a = -5;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 160, result);
  // Patch 15-6 # 161
  constant_a = -4;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 161, result);
  // Patch 15-7 # 162
  constant_a = -3;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 162, result);
  // Patch 15-8 # 163
  constant_a = -2;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 163, result);
  // Patch 15-9 # 164
  constant_a = -1;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 164, result);
  // Patch 15-10 # 165
  constant_a = 0;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 165, result);
  // Patch 15-11 # 166
  constant_a = 1;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 166, result);
  // Patch 15-12 # 167
  constant_a = 2;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 167, result);
  // Patch 15-13 # 168
  constant_a = 3;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 168, result);
  // Patch 15-14 # 169
  constant_a = 4;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 169, result);
  // Patch 15-15 # 170
  constant_a = 5;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 170, result);
  // Patch 15-16 # 171
  constant_a = 6;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 171, result);
  // Patch 15-17 # 172
  constant_a = 7;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 172, result);
  // Patch 15-18 # 173
  constant_a = 8;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 173, result);
  // Patch 15-19 # 174
  constant_a = 9;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 174, result);
  // Patch 15-20 # 175
  constant_a = 10;
  result = (constant_a < initial_read);
  uni_klee_add_patch(patch_results, 175, result);
  // Patch 16-0 # 176
  constant_a = -10;
  result = (constant_a < bufsize);
  uni_klee_add_patch(patch_results, 176, result);
  // Patch 16-1 # 177
  constant_a = -9;
  result = (constant_a < bufsize);
  uni_klee_add_patch(patch_results, 177, result);
  // Patch 16-2 # 178
  constant_a = -8;
  result = (constant_a < bufsize);
  uni_klee_add_patch(patch_results, 178, result);
  // Patch 16-3 # 179
  constant_a = -7;
  result = (constant_a < bufsize);
  uni_klee_add_patch(patch_results, 179, result);
  // Patch 16-4 # 180
  constant_a = -6;
  result = (constant_a < bufsize);
  uni_klee_add_patch(patch_results, 180, result);
  // Patch 16-5 # 181
  constant_a = -5;
  result = (constant_a < bufsize);
  uni_klee_add_patch(patch_results, 181, result);
  // Patch 16-6 # 182
  constant_a = -4;
  result = (constant_a < bufsize);
  uni_klee_add_patch(patch_results, 182, result);
  // Patch 16-7 # 183
  constant_a = -3;
  result = (constant_a < bufsize);
  uni_klee_add_patch(patch_results, 183, result);
  // Patch 16-8 # 184
  constant_a = -2;
  result = (constant_a < bufsize);
  uni_klee_add_patch(patch_results, 184, result);
  // Patch 16-9 # 185
  constant_a = -1;
  result = (constant_a < bufsize);
  uni_klee_add_patch(patch_results, 185, result);
  // Patch 16-10 # 186
  constant_a = 0;
  result = (constant_a < bufsize);
  uni_klee_add_patch(patch_results, 186, result);
  // Patch 16-11 # 187
  constant_a = 1;
  result = (constant_a < bufsize);
  uni_klee_add_patch(patch_results, 187, result);
  // Patch 16-12 # 188
  constant_a = 2;
  result = (constant_a < bufsize);
  uni_klee_add_patch(patch_results, 188, result);
  // Patch 16-13 # 189
  constant_a = 3;
  result = (constant_a < bufsize);
  uni_klee_add_patch(patch_results, 189, result);
  // Patch 16-14 # 190
  constant_a = 4;
  result = (constant_a < bufsize);
  uni_klee_add_patch(patch_results, 190, result);
  // Patch 16-15 # 191
  constant_a = 5;
  result = (constant_a < bufsize);
  uni_klee_add_patch(patch_results, 191, result);
  // Patch 16-16 # 192
  constant_a = 6;
  result = (constant_a < bufsize);
  uni_klee_add_patch(patch_results, 192, result);
  // Patch 16-17 # 193
  constant_a = 7;
  result = (constant_a < bufsize);
  uni_klee_add_patch(patch_results, 193, result);
  // Patch 16-18 # 194
  constant_a = 8;
  result = (constant_a < bufsize);
  uni_klee_add_patch(patch_results, 194, result);
  // Patch 16-19 # 195
  constant_a = 9;
  result = (constant_a < bufsize);
  uni_klee_add_patch(patch_results, 195, result);
  // Patch 16-20 # 196
  constant_a = 10;
  result = (constant_a < bufsize);
  uni_klee_add_patch(patch_results, 196, result);
  // Patch 17-0 # 197
  constant_a = -10;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 197, result);
  // Patch 17-1 # 198
  constant_a = -9;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 198, result);
  // Patch 17-2 # 199
  constant_a = -8;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 199, result);
  // Patch 17-3 # 200
  constant_a = -7;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 200, result);
  // Patch 17-4 # 201
  constant_a = -6;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 201, result);
  // Patch 17-5 # 202
  constant_a = -5;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 202, result);
  // Patch 17-6 # 203
  constant_a = -4;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 203, result);
  // Patch 17-7 # 204
  constant_a = -3;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 204, result);
  // Patch 17-8 # 205
  constant_a = -2;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 205, result);
  // Patch 17-9 # 206
  constant_a = -1;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 206, result);
  // Patch 17-10 # 207
  constant_a = 0;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 207, result);
  // Patch 17-11 # 208
  constant_a = 1;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 208, result);
  // Patch 17-12 # 209
  constant_a = 2;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 209, result);
  // Patch 17-13 # 210
  constant_a = 3;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 210, result);
  // Patch 17-14 # 211
  constant_a = 4;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 211, result);
  // Patch 17-15 # 212
  constant_a = 5;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 212, result);
  // Patch 17-16 # 213
  constant_a = 6;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 213, result);
  // Patch 17-17 # 214
  constant_a = 7;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 214, result);
  // Patch 17-18 # 215
  constant_a = 8;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 215, result);
  // Patch 17-19 # 216
  constant_a = 9;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 216, result);
  // Patch 17-20 # 217
  constant_a = 10;
  result = (start < constant_a);
  uni_klee_add_patch(patch_results, 217, result);
  // Patch 18-0 # 218
  constant_a = -10;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 218, result);
  // Patch 18-1 # 219
  constant_a = -9;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 219, result);
  // Patch 18-2 # 220
  constant_a = -8;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 220, result);
  // Patch 18-3 # 221
  constant_a = -7;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 221, result);
  // Patch 18-4 # 222
  constant_a = -6;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 222, result);
  // Patch 18-5 # 223
  constant_a = -5;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 223, result);
  // Patch 18-6 # 224
  constant_a = -4;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 224, result);
  // Patch 18-7 # 225
  constant_a = -3;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 225, result);
  // Patch 18-8 # 226
  constant_a = -2;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 226, result);
  // Patch 18-9 # 227
  constant_a = -1;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 227, result);
  // Patch 18-10 # 228
  constant_a = 0;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 228, result);
  // Patch 18-11 # 229
  constant_a = 1;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 229, result);
  // Patch 18-12 # 230
  constant_a = 2;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 230, result);
  // Patch 18-13 # 231
  constant_a = 3;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 231, result);
  // Patch 18-14 # 232
  constant_a = 4;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 232, result);
  // Patch 18-15 # 233
  constant_a = 5;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 233, result);
  // Patch 18-16 # 234
  constant_a = 6;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 234, result);
  // Patch 18-17 # 235
  constant_a = 7;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 235, result);
  // Patch 18-18 # 236
  constant_a = 8;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 236, result);
  // Patch 18-19 # 237
  constant_a = 9;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 237, result);
  // Patch 18-20 # 238
  constant_a = 10;
  result = (initial_read < constant_a);
  uni_klee_add_patch(patch_results, 238, result);
  // Patch 19-0 # 239
  constant_a = -10;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 239, result);
  // Patch 19-1 # 240
  constant_a = -9;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 240, result);
  // Patch 19-2 # 241
  constant_a = -8;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 241, result);
  // Patch 19-3 # 242
  constant_a = -7;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 242, result);
  // Patch 19-4 # 243
  constant_a = -6;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 243, result);
  // Patch 19-5 # 244
  constant_a = -5;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 244, result);
  // Patch 19-6 # 245
  constant_a = -4;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 245, result);
  // Patch 19-7 # 246
  constant_a = -3;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 246, result);
  // Patch 19-8 # 247
  constant_a = -2;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 247, result);
  // Patch 19-9 # 248
  constant_a = -1;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 248, result);
  // Patch 19-10 # 249
  constant_a = 0;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 249, result);
  // Patch 19-11 # 250
  constant_a = 1;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 250, result);
  // Patch 19-12 # 251
  constant_a = 2;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 251, result);
  // Patch 19-13 # 252
  constant_a = 3;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 252, result);
  // Patch 19-14 # 253
  constant_a = 4;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 253, result);
  // Patch 19-15 # 254
  constant_a = 5;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 254, result);
  // Patch 19-16 # 255
  constant_a = 6;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 255, result);
  // Patch 19-17 # 256
  constant_a = 7;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 256, result);
  // Patch 19-18 # 257
  constant_a = 8;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 257, result);
  // Patch 19-19 # 258
  constant_a = 9;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 258, result);
  // Patch 19-20 # 259
  constant_a = 10;
  result = (bufsize < constant_a);
  uni_klee_add_patch(patch_results, 259, result);
  // Patch 20-0 # 260
  result = (bufsize <= start);
  uni_klee_add_patch(patch_results, 260, result);
  // Patch 21-0 # 261
  constant_a = -10;
  result = (constant_a <= start);
  uni_klee_add_patch(patch_results, 261, result);
  // Patch 21-1 # 262
  constant_a = -9;
  result = (constant_a <= start);
  uni_klee_add_patch(patch_results, 262, result);
  // Patch 21-2 # 263
  constant_a = -8;
  result = (constant_a <= start);
  uni_klee_add_patch(patch_results, 263, result);
  // Patch 21-3 # 264
  constant_a = -7;
  result = (constant_a <= start);
  uni_klee_add_patch(patch_results, 264, result);
  // Patch 21-4 # 265
  constant_a = -6;
  result = (constant_a <= start);
  uni_klee_add_patch(patch_results, 265, result);
  // Patch 21-5 # 266
  constant_a = -5;
  result = (constant_a <= start);
  uni_klee_add_patch(patch_results, 266, result);
  // Patch 21-6 # 267
  constant_a = -4;
  result = (constant_a <= start);
  uni_klee_add_patch(patch_results, 267, result);
  // Patch 21-7 # 268
  constant_a = -3;
  result = (constant_a <= start);
  uni_klee_add_patch(patch_results, 268, result);
  // Patch 21-8 # 269
  constant_a = -2;
  result = (constant_a <= start);
  uni_klee_add_patch(patch_results, 269, result);
  // Patch 21-9 # 270
  constant_a = -1;
  result = (constant_a <= start);
  uni_klee_add_patch(patch_results, 270, result);
  // Patch 21-10 # 271
  constant_a = 0;
  result = (constant_a <= start);
  uni_klee_add_patch(patch_results, 271, result);
  // Patch 21-11 # 272
  constant_a = 1;
  result = (constant_a <= start);
  uni_klee_add_patch(patch_results, 272, result);
  // Patch 21-12 # 273
  constant_a = 2;
  result = (constant_a <= start);
  uni_klee_add_patch(patch_results, 273, result);
  // Patch 21-13 # 274
  constant_a = 3;
  result = (constant_a <= start);
  uni_klee_add_patch(patch_results, 274, result);
  // Patch 21-14 # 275
  constant_a = 4;
  result = (constant_a <= start);
  uni_klee_add_patch(patch_results, 275, result);
  // Patch 21-15 # 276
  constant_a = 5;
  result = (constant_a <= start);
  uni_klee_add_patch(patch_results, 276, result);
  // Patch 21-16 # 277
  constant_a = 6;
  result = (constant_a <= start);
  uni_klee_add_patch(patch_results, 277, result);
  // Patch 21-17 # 278
  constant_a = 7;
  result = (constant_a <= start);
  uni_klee_add_patch(patch_results, 278, result);
  // Patch 21-18 # 279
  constant_a = 8;
  result = (constant_a <= start);
  uni_klee_add_patch(patch_results, 279, result);
  // Patch 21-19 # 280
  constant_a = 9;
  result = (constant_a <= start);
  uni_klee_add_patch(patch_results, 280, result);
  // Patch 21-20 # 281
  constant_a = 10;
  result = (constant_a <= start);
  uni_klee_add_patch(patch_results, 281, result);
  // Patch 22-0 # 282
  result = (start <= initial_read);
  uni_klee_add_patch(patch_results, 282, result);
  // Patch 23-0 # 283
  result = (bufsize <= initial_read);
  uni_klee_add_patch(patch_results, 283, result);
  // Patch 24-0 # 284
  constant_a = -10;
  result = (constant_a <= initial_read);
  uni_klee_add_patch(patch_results, 284, result);
  // Patch 24-1 # 285
  constant_a = -9;
  result = (constant_a <= initial_read);
  uni_klee_add_patch(patch_results, 285, result);
  // Patch 24-2 # 286
  constant_a = -8;
  result = (constant_a <= initial_read);
  uni_klee_add_patch(patch_results, 286, result);
  // Patch 24-3 # 287
  constant_a = -7;
  result = (constant_a <= initial_read);
  uni_klee_add_patch(patch_results, 287, result);
  // Patch 24-4 # 288
  constant_a = -6;
  result = (constant_a <= initial_read);
  uni_klee_add_patch(patch_results, 288, result);
  // Patch 24-5 # 289
  constant_a = -5;
  result = (constant_a <= initial_read);
  uni_klee_add_patch(patch_results, 289, result);
  // Patch 24-6 # 290
  constant_a = -4;
  result = (constant_a <= initial_read);
  uni_klee_add_patch(patch_results, 290, result);
  // Patch 24-7 # 291
  constant_a = -3;
  result = (constant_a <= initial_read);
  uni_klee_add_patch(patch_results, 291, result);
  // Patch 24-8 # 292
  constant_a = -2;
  result = (constant_a <= initial_read);
  uni_klee_add_patch(patch_results, 292, result);
  // Patch 24-9 # 293
  constant_a = -1;
  result = (constant_a <= initial_read);
  uni_klee_add_patch(patch_results, 293, result);
  // Patch 24-10 # 294
  constant_a = 0;
  result = (constant_a <= initial_read);
  uni_klee_add_patch(patch_results, 294, result);
  // Patch 24-11 # 295
  constant_a = 1;
  result = (constant_a <= initial_read);
  uni_klee_add_patch(patch_results, 295, result);
  // Patch 24-12 # 296
  constant_a = 2;
  result = (constant_a <= initial_read);
  uni_klee_add_patch(patch_results, 296, result);
  // Patch 24-13 # 297
  constant_a = 3;
  result = (constant_a <= initial_read);
  uni_klee_add_patch(patch_results, 297, result);
  // Patch 24-14 # 298
  constant_a = 4;
  result = (constant_a <= initial_read);
  uni_klee_add_patch(patch_results, 298, result);
  // Patch 24-15 # 299
  constant_a = 5;
  result = (constant_a <= initial_read);
  uni_klee_add_patch(patch_results, 299, result);
  // Patch 24-16 # 300
  constant_a = 6;
  result = (constant_a <= initial_read);
  uni_klee_add_patch(patch_results, 300, result);
  // Patch 24-17 # 301
  constant_a = 7;
  result = (constant_a <= initial_read);
  uni_klee_add_patch(patch_results, 301, result);
  // Patch 24-18 # 302
  constant_a = 8;
  result = (constant_a <= initial_read);
  uni_klee_add_patch(patch_results, 302, result);
  // Patch 24-19 # 303
  constant_a = 9;
  result = (constant_a <= initial_read);
  uni_klee_add_patch(patch_results, 303, result);
  // Patch 24-20 # 304
  constant_a = 10;
  result = (constant_a <= initial_read);
  uni_klee_add_patch(patch_results, 304, result);
  // Patch 25-0 # 305
  constant_a = -10;
  result = (constant_a <= bufsize);
  uni_klee_add_patch(patch_results, 305, result);
  // Patch 25-1 # 306
  constant_a = -9;
  result = (constant_a <= bufsize);
  uni_klee_add_patch(patch_results, 306, result);
  // Patch 25-2 # 307
  constant_a = -8;
  result = (constant_a <= bufsize);
  uni_klee_add_patch(patch_results, 307, result);
  // Patch 25-3 # 308
  constant_a = -7;
  result = (constant_a <= bufsize);
  uni_klee_add_patch(patch_results, 308, result);
  // Patch 25-4 # 309
  constant_a = -6;
  result = (constant_a <= bufsize);
  uni_klee_add_patch(patch_results, 309, result);
  // Patch 25-5 # 310
  constant_a = -5;
  result = (constant_a <= bufsize);
  uni_klee_add_patch(patch_results, 310, result);
  // Patch 25-6 # 311
  constant_a = -4;
  result = (constant_a <= bufsize);
  uni_klee_add_patch(patch_results, 311, result);
  // Patch 25-7 # 312
  constant_a = -3;
  result = (constant_a <= bufsize);
  uni_klee_add_patch(patch_results, 312, result);
  // Patch 25-8 # 313
  constant_a = -2;
  result = (constant_a <= bufsize);
  uni_klee_add_patch(patch_results, 313, result);
  // Patch 25-9 # 314
  constant_a = -1;
  result = (constant_a <= bufsize);
  uni_klee_add_patch(patch_results, 314, result);
  // Patch 25-10 # 315
  constant_a = 0;
  result = (constant_a <= bufsize);
  uni_klee_add_patch(patch_results, 315, result);
  // Patch 25-11 # 316
  constant_a = 1;
  result = (constant_a <= bufsize);
  uni_klee_add_patch(patch_results, 316, result);
  // Patch 25-12 # 317
  constant_a = 2;
  result = (constant_a <= bufsize);
  uni_klee_add_patch(patch_results, 317, result);
  // Patch 25-13 # 318
  constant_a = 3;
  result = (constant_a <= bufsize);
  uni_klee_add_patch(patch_results, 318, result);
  // Patch 25-14 # 319
  constant_a = 4;
  result = (constant_a <= bufsize);
  uni_klee_add_patch(patch_results, 319, result);
  // Patch 25-15 # 320
  constant_a = 5;
  result = (constant_a <= bufsize);
  uni_klee_add_patch(patch_results, 320, result);
  // Patch 25-16 # 321
  constant_a = 6;
  result = (constant_a <= bufsize);
  uni_klee_add_patch(patch_results, 321, result);
  // Patch 25-17 # 322
  constant_a = 7;
  result = (constant_a <= bufsize);
  uni_klee_add_patch(patch_results, 322, result);
  // Patch 25-18 # 323
  constant_a = 8;
  result = (constant_a <= bufsize);
  uni_klee_add_patch(patch_results, 323, result);
  // Patch 25-19 # 324
  constant_a = 9;
  result = (constant_a <= bufsize);
  uni_klee_add_patch(patch_results, 324, result);
  // Patch 25-20 # 325
  constant_a = 10;
  result = (constant_a <= bufsize);
  uni_klee_add_patch(patch_results, 325, result);
  // Patch 26-0 # 326
  constant_a = -10;
  result = (start <= constant_a);
  uni_klee_add_patch(patch_results, 326, result);
  // Patch 26-1 # 327
  constant_a = -9;
  result = (start <= constant_a);
  uni_klee_add_patch(patch_results, 327, result);
  // Patch 26-2 # 328
  constant_a = -8;
  result = (start <= constant_a);
  uni_klee_add_patch(patch_results, 328, result);
  // Patch 26-3 # 329
  constant_a = -7;
  result = (start <= constant_a);
  uni_klee_add_patch(patch_results, 329, result);
  // Patch 26-4 # 330
  constant_a = -6;
  result = (start <= constant_a);
  uni_klee_add_patch(patch_results, 330, result);
  // Patch 26-5 # 331
  constant_a = -5;
  result = (start <= constant_a);
  uni_klee_add_patch(patch_results, 331, result);
  // Patch 26-6 # 332
  constant_a = -4;
  result = (start <= constant_a);
  uni_klee_add_patch(patch_results, 332, result);
  // Patch 26-7 # 333
  constant_a = -3;
  result = (start <= constant_a);
  uni_klee_add_patch(patch_results, 333, result);
  // Patch 26-8 # 334
  constant_a = -2;
  result = (start <= constant_a);
  uni_klee_add_patch(patch_results, 334, result);
  // Patch 26-9 # 335
  constant_a = -1;
  result = (start <= constant_a);
  uni_klee_add_patch(patch_results, 335, result);
  // Patch 26-10 # 336
  constant_a = 0;
  result = (start <= constant_a);
  uni_klee_add_patch(patch_results, 336, result);
  // Patch 26-11 # 337
  constant_a = 1;
  result = (start <= constant_a);
  uni_klee_add_patch(patch_results, 337, result);
  // Patch 26-12 # 338
  constant_a = 2;
  result = (start <= constant_a);
  uni_klee_add_patch(patch_results, 338, result);
  // Patch 26-13 # 339
  constant_a = 3;
  result = (start <= constant_a);
  uni_klee_add_patch(patch_results, 339, result);
  // Patch 26-14 # 340
  constant_a = 4;
  result = (start <= constant_a);
  uni_klee_add_patch(patch_results, 340, result);
  // Patch 26-15 # 341
  constant_a = 5;
  result = (start <= constant_a);
  uni_klee_add_patch(patch_results, 341, result);
  // Patch 26-16 # 342
  constant_a = 6;
  result = (start <= constant_a);
  uni_klee_add_patch(patch_results, 342, result);
  // Patch 26-17 # 343
  constant_a = 7;
  result = (start <= constant_a);
  uni_klee_add_patch(patch_results, 343, result);
  // Patch 26-18 # 344
  constant_a = 8;
  result = (start <= constant_a);
  uni_klee_add_patch(patch_results, 344, result);
  // Patch 26-19 # 345
  constant_a = 9;
  result = (start <= constant_a);
  uni_klee_add_patch(patch_results, 345, result);
  // Patch 26-20 # 346
  constant_a = 10;
  result = (start <= constant_a);
  uni_klee_add_patch(patch_results, 346, result);
  // Patch 27-0 # 347
  constant_a = -10;
  result = (initial_read <= constant_a);
  uni_klee_add_patch(patch_results, 347, result);
  // Patch 27-1 # 348
  constant_a = -9;
  result = (initial_read <= constant_a);
  uni_klee_add_patch(patch_results, 348, result);
  // Patch 27-2 # 349
  constant_a = -8;
  result = (initial_read <= constant_a);
  uni_klee_add_patch(patch_results, 349, result);
  // Patch 27-3 # 350
  constant_a = -7;
  result = (initial_read <= constant_a);
  uni_klee_add_patch(patch_results, 350, result);
  // Patch 27-4 # 351
  constant_a = -6;
  result = (initial_read <= constant_a);
  uni_klee_add_patch(patch_results, 351, result);
  // Patch 27-5 # 352
  constant_a = -5;
  result = (initial_read <= constant_a);
  uni_klee_add_patch(patch_results, 352, result);
  // Patch 27-6 # 353
  constant_a = -4;
  result = (initial_read <= constant_a);
  uni_klee_add_patch(patch_results, 353, result);
  // Patch 27-7 # 354
  constant_a = -3;
  result = (initial_read <= constant_a);
  uni_klee_add_patch(patch_results, 354, result);
  // Patch 27-8 # 355
  constant_a = -2;
  result = (initial_read <= constant_a);
  uni_klee_add_patch(patch_results, 355, result);
  // Patch 27-9 # 356
  constant_a = -1;
  result = (initial_read <= constant_a);
  uni_klee_add_patch(patch_results, 356, result);
  // Patch 27-10 # 357
  constant_a = 0;
  result = (initial_read <= constant_a);
  uni_klee_add_patch(patch_results, 357, result);
  // Patch 27-11 # 358
  constant_a = 1;
  result = (initial_read <= constant_a);
  uni_klee_add_patch(patch_results, 358, result);
  // Patch 27-12 # 359
  constant_a = 2;
  result = (initial_read <= constant_a);
  uni_klee_add_patch(patch_results, 359, result);
  // Patch 27-13 # 360
  constant_a = 3;
  result = (initial_read <= constant_a);
  uni_klee_add_patch(patch_results, 360, result);
  // Patch 27-14 # 361
  constant_a = 4;
  result = (initial_read <= constant_a);
  uni_klee_add_patch(patch_results, 361, result);
  // Patch 27-15 # 362
  constant_a = 5;
  result = (initial_read <= constant_a);
  uni_klee_add_patch(patch_results, 362, result);
  // Patch 27-16 # 363
  constant_a = 6;
  result = (initial_read <= constant_a);
  uni_klee_add_patch(patch_results, 363, result);
  // Patch 27-17 # 364
  constant_a = 7;
  result = (initial_read <= constant_a);
  uni_klee_add_patch(patch_results, 364, result);
  // Patch 27-18 # 365
  constant_a = 8;
  result = (initial_read <= constant_a);
  uni_klee_add_patch(patch_results, 365, result);
  // Patch 27-19 # 366
  constant_a = 9;
  result = (initial_read <= constant_a);
  uni_klee_add_patch(patch_results, 366, result);
  // Patch 27-20 # 367
  constant_a = 10;
  result = (initial_read <= constant_a);
  uni_klee_add_patch(patch_results, 367, result);
  // Patch 28-0 # 368
  constant_a = -10;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 368, result);
  // Patch 28-1 # 369
  constant_a = -9;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 369, result);
  // Patch 28-2 # 370
  constant_a = -8;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 370, result);
  // Patch 28-3 # 371
  constant_a = -7;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 371, result);
  // Patch 28-4 # 372
  constant_a = -6;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 372, result);
  // Patch 28-5 # 373
  constant_a = -5;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 373, result);
  // Patch 28-6 # 374
  constant_a = -4;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 374, result);
  // Patch 28-7 # 375
  constant_a = -3;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 375, result);
  // Patch 28-8 # 376
  constant_a = -2;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 376, result);
  // Patch 28-9 # 377
  constant_a = -1;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 377, result);
  // Patch 28-10 # 378
  constant_a = 0;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 378, result);
  // Patch 28-11 # 379
  constant_a = 1;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 379, result);
  // Patch 28-12 # 380
  constant_a = 2;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 380, result);
  // Patch 28-13 # 381
  constant_a = 3;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 381, result);
  // Patch 28-14 # 382
  constant_a = 4;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 382, result);
  // Patch 28-15 # 383
  constant_a = 5;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 383, result);
  // Patch 28-16 # 384
  constant_a = 6;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 384, result);
  // Patch 28-17 # 385
  constant_a = 7;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 385, result);
  // Patch 28-18 # 386
  constant_a = 8;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 386, result);
  // Patch 28-19 # 387
  constant_a = 9;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 387, result);
  // Patch 28-20 # 388
  constant_a = 10;
  result = (bufsize <= constant_a);
  uni_klee_add_patch(patch_results, 388, result);
  // Patch correct # 389
  result = (start < initial_read);
  uni_klee_add_patch(patch_results, 389, result);
  klee_select_patch(&uni_klee_patch_id);
  return uni_klee_choice(patch_results, uni_klee_patch_id);
}
// UNI_KLEE_END
/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "split"

#define AUTHORS \
  proper_name ("Torbjorn Granlund"), \
  proper_name ("Richard M. Stallman")

/* Shell command to filter through, instead of creating files.  */
static char const *filter_command;

/* Process ID of the filter.  */
static int filter_pid;

/* Array of open pipes.  */
static int *open_pipes;
static size_t open_pipes_alloc;
static size_t n_open_pipes;

/* Blocked signals.  */
static sigset_t oldblocked;
static sigset_t newblocked;

/* Base name of output files.  */
static char const *outbase;

/* Name of output files.  */
static char *outfile;

/* Pointer to the end of the prefix in OUTFILE.
   Suffixes are inserted here.  */
static char *outfile_mid;

/* Generate new suffix when suffixes are exhausted.  */
static bool suffix_auto = true;

/* Length of OUTFILE's suffix.  */
static size_t suffix_length;

/* Alphabet of characters to use in suffix.  */
static char const *suffix_alphabet = "abcdefghijklmnopqrstuvwxyz";

/* Numerical suffix start value.  */
static const char *numeric_suffix_start;

/* Additional suffix to append to output file names.  */
static char const *additional_suffix;

/* Name of input file.  May be "-".  */
static char *infile;

/* stat buf for input file.  */
static struct stat in_stat_buf;

/* Descriptor on which output file is open.  */
static int output_desc = -1;

/* If true, print a diagnostic on standard error just before each
   output file is opened. */
static bool verbose;

/* If true, don't generate zero length output files. */
static bool elide_empty_files;

/* If true, in round robin mode, immediately copy
   input to output, which is much slower, so disabled by default.  */
static bool unbuffered;

/* The character marking end of line.  Defaults to \n below.  */
static int eolchar = -1;

/* The split mode to use.  */
enum Split_type
{
  type_undef, type_bytes, type_byteslines, type_lines, type_digits,
  type_chunk_bytes, type_chunk_lines, type_rr
};

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
enum
{
  VERBOSE_OPTION = CHAR_MAX + 1,
  FILTER_OPTION,
  IO_BLKSIZE_OPTION,
  ADDITIONAL_SUFFIX_OPTION
};

static struct option const longopts[] =
{
  {"bytes", required_argument, NULL, 'b'},
  {"lines", required_argument, NULL, 'l'},
  {"line-bytes", required_argument, NULL, 'C'},
  {"number", required_argument, NULL, 'n'},
  {"elide-empty-files", no_argument, NULL, 'e'},
  {"unbuffered", no_argument, NULL, 'u'},
  {"suffix-length", required_argument, NULL, 'a'},
  {"additional-suffix", required_argument, NULL,
   ADDITIONAL_SUFFIX_OPTION},
  {"numeric-suffixes", optional_argument, NULL, 'd'},
  {"filter", required_argument, NULL, FILTER_OPTION},
  {"verbose", no_argument, NULL, VERBOSE_OPTION},
  {"separator", required_argument, NULL, 't'},
  {"-io-blksize", required_argument, NULL,
   IO_BLKSIZE_OPTION}, /* do not document */
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

/* Return true if the errno value, ERR, is ignorable.  */
static inline bool
ignorable (int err)
{
  return filter_command && err == EPIPE;
}

static void
set_suffix_length (uintmax_t n_units, enum Split_type split_type)
{
#define DEFAULT_SUFFIX_LENGTH 2

  uintmax_t suffix_needed = 0;

  /* The suffix auto length feature is incompatible with
     a user specified start value as the generated suffixes
     are not all consecutive.  */
  if (numeric_suffix_start)
    suffix_auto = false;

  /* Auto-calculate the suffix length if the number of files is given.  */
  if (split_type == type_chunk_bytes || split_type == type_chunk_lines
      || split_type == type_rr)
    {
      uintmax_t n_units_end = n_units;
      if (numeric_suffix_start)
        {
          uintmax_t n_start;
          strtol_error e = xstrtoumax (numeric_suffix_start, NULL, 10,
                                       &n_start, "");
          if (e == LONGINT_OK && n_start <= UINTMAX_MAX - n_units)
            {
              /* Restrict auto adjustment so we don't keep
                 incrementing a suffix size arbitrarily,
                 as that would break sort order for files
                 generated from multiple split runs.  */
              if (n_start < n_units)
                n_units_end += n_start;
            }

        }
      size_t alphabet_len = strlen (suffix_alphabet);
      bool alphabet_slop = (n_units_end % alphabet_len) != 0;
      while (n_units_end /= alphabet_len)
        suffix_needed++;
      suffix_needed += alphabet_slop;
      suffix_auto = false;
    }

  if (suffix_length)            /* set by user */
    {
      if (suffix_length < suffix_needed)
        {
          die (EXIT_FAILURE, 0,
               _("the suffix length needs to be at least %"PRIuMAX),
               suffix_needed);
        }
      suffix_auto = false;
      return;
    }
  else
    suffix_length = MAX (DEFAULT_SUFFIX_LENGTH, suffix_needed);
}

void
usage (int status)
{
  if (status != EXIT_SUCCESS)
    emit_try_help ();
  else
    {
      printf (_("\
Usage: %s [OPTION]... [FILE [PREFIX]]\n\
"),
              program_name);
      fputs (_("\
Output pieces of FILE to PREFIXaa, PREFIXab, ...;\n\
default size is 1000 lines, and default PREFIX is 'x'.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      fprintf (stdout, _("\
  -a, --suffix-length=N   generate suffixes of length N (default %d)\n\
      --additional-suffix=SUFFIX  append an additional SUFFIX to file names\n\
  -b, --bytes=SIZE        put SIZE bytes per output file\n\
  -C, --line-bytes=SIZE   put at most SIZE bytes of records per output file\n\
  -d                      use numeric suffixes starting at 0, not alphabetic\n\
      --numeric-suffixes[=FROM]  same as -d, but allow setting the start value\
\n\
  -e, --elide-empty-files  do not generate empty output files with '-n'\n\
      --filter=COMMAND    write to shell COMMAND; file name is $FILE\n\
  -l, --lines=NUMBER      put NUMBER lines/records per output file\n\
  -n, --number=CHUNKS     generate CHUNKS output files; see explanation below\n\
  -t, --separator=SEP     use SEP instead of newline as the record separator;\n\
                            '\\0' (zero) specifies the NUL character\n\
  -u, --unbuffered        immediately copy input to output with '-n r/...'\n\
"), DEFAULT_SUFFIX_LENGTH);
      fputs (_("\
      --verbose           print a diagnostic just before each\n\
                            output file is opened\n\
"), stdout);
      fputs (HELP_OPTION_DESCRIPTION, stdout);
      fputs (VERSION_OPTION_DESCRIPTION, stdout);
      emit_size_note ();
      fputs (_("\n\
CHUNKS may be:\n\
  N       split into N files based on size of input\n\
  K/N     output Kth of N to stdout\n\
  l/N     split into N files without splitting lines/records\n\
  l/K/N   output Kth of N to stdout without splitting lines/records\n\
  r/N     like 'l' but use round robin distribution\n\
  r/K/N   likewise but only output Kth of N to stdout\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Return the number of bytes that can be read from FD with status ST.
   Store up to the first BUFSIZE bytes of the file's data into BUF,
   and advance the file position by the number of bytes read.  On
   input error, set errno and return -1.  */

static off_t
input_file_size (int fd, struct stat const *st, char *buf, size_t bufsize)
{
  off_t cur = lseek (fd, 0, SEEK_CUR);
  if (cur < 0)
    {
      if (errno == ESPIPE)
        errno = 0; /* Suppress confusing seek error.  */
      return -1;
    }

  off_t size = 0;
  do
    {
      size_t n_read = safe_read (fd, buf + size, bufsize - size);
      if (n_read == 0)
        return size;
      if (n_read == SAFE_READ_ERROR)
        return -1;
      size += n_read;
    }
  while (size < bufsize);

  /* Note we check st_size _after_ the read() above
     because /proc files on GNU/Linux are seekable
     but have st_size == 0.  */
  if (st->st_size == 0)
    {
      /* We've filled the buffer, from a seekable file,
         which has an st_size==0, E.g., /dev/zero on GNU/Linux.
         Assume there is no limit to file size.  */
      errno = EOVERFLOW;
      return -1;
    }

  cur += size;
  off_t end;
  if (usable_st_size (st) && cur <= st->st_size)
    end = st->st_size;
  else
    {
      end = lseek (fd, 0, SEEK_END);
      if (end < 0)
        return -1;
      if (end != cur)
        {
          if (lseek (fd, cur, SEEK_SET) < 0)
            return -1;
          if (end < cur)
            end = cur;
        }
    }

  size += end - cur;
  if (size == OFF_T_MAX)
    {
      /* E.g., /dev/zero on GNU/Hurd.  */
      errno = EOVERFLOW;
      return -1;
    }

  return size;
}

/* Compute the next sequential output file name and store it into the
   string 'outfile'.  */

static void
next_file_name (void)
{
  /* Index in suffix_alphabet of each character in the suffix.  */
  static size_t *sufindex;
  static size_t outbase_length;
  static size_t outfile_length;
  static size_t addsuf_length;

  if (! outfile)
    {
      bool widen;

new_name:
      widen = !! outfile_length;

      if (! widen)
        {
          /* Allocate and initialize the first file name.  */

          outbase_length = strlen (outbase);
          addsuf_length = additional_suffix ? strlen (additional_suffix) : 0;
          outfile_length = outbase_length + suffix_length + addsuf_length;
        }
      else
        {
          /* Reallocate and initialize a new wider file name.
             We do this by subsuming the unchanging part of
             the generated suffix into the prefix (base), and
             reinitializing the now one longer suffix.  */

          outfile_length += 2;
          suffix_length++;
        }

      if (outfile_length + 1 < outbase_length)
        xalloc_die ();
      outfile = xrealloc (outfile, outfile_length + 1);

      if (! widen)
        memcpy (outfile, outbase, outbase_length);
      else
        {
          /* Append the last alphabet character to the file name prefix.  */
          outfile[outbase_length] = suffix_alphabet[sufindex[0]];
          outbase_length++;
        }

      outfile_mid = outfile + outbase_length;
      memset (outfile_mid, suffix_alphabet[0], suffix_length);
      if (additional_suffix)
        memcpy (outfile_mid + suffix_length, additional_suffix, addsuf_length);
      outfile[outfile_length] = 0;

      free (sufindex);
      sufindex = xcalloc (suffix_length, sizeof *sufindex);

      if (numeric_suffix_start)
        {
          assert (! widen);

          /* Update the output file name.  */
          size_t i = strlen (numeric_suffix_start);
          memcpy (outfile_mid + suffix_length - i, numeric_suffix_start, i);

          /* Update the suffix index.  */
          size_t *sufindex_end = sufindex + suffix_length;
          while (i-- != 0)
            *--sufindex_end = numeric_suffix_start[i] - '0';
        }

#if ! _POSIX_NO_TRUNC && HAVE_PATHCONF && defined _PC_NAME_MAX
      /* POSIX requires that if the output file name is too long for
         its directory, 'split' must fail without creating any files.
         This must be checked for explicitly on operating systems that
         silently truncate file names.  */
      {
        char *dir = dir_name (outfile);
        long name_max = pathconf (dir, _PC_NAME_MAX);
        if (0 <= name_max && name_max < base_len (last_component (outfile)))
          die (EXIT_FAILURE, ENAMETOOLONG, "%s", quotef (outfile));
        free (dir);
      }
#endif
    }
  else
    {
      /* Increment the suffix in place, if possible.  */

      size_t i = suffix_length;
      while (i-- != 0)
        {
          sufindex[i]++;
          if (suffix_auto && i == 0 && ! suffix_alphabet[sufindex[0] + 1])
            goto new_name;
          outfile_mid[i] = suffix_alphabet[sufindex[i]];
          if (outfile_mid[i])
            return;
          sufindex[i] = 0;
          outfile_mid[i] = suffix_alphabet[sufindex[i]];
        }
      die (EXIT_FAILURE, 0, _("output file suffixes exhausted"));
    }
}

/* Create or truncate a file.  */

static int
create (const char *name)
{
  if (!filter_command)
    {
      if (verbose)
        fprintf (stdout, _("creating file %s\n"), quoteaf (name));

      int fd = open (name, O_WRONLY | O_CREAT | O_BINARY, MODE_RW_UGO);
      if (fd < 0)
        return fd;
      struct stat out_stat_buf;
      if (fstat (fd, &out_stat_buf) != 0)
        die (EXIT_FAILURE, errno, _("failed to stat %s"), quoteaf (name));
      if (SAME_INODE (in_stat_buf, out_stat_buf))
        die (EXIT_FAILURE, 0, _("%s would overwrite input; aborting"),
             quoteaf (name));
      if (ftruncate (fd, 0) != 0)
        die (EXIT_FAILURE, errno, _("%s: error truncating"), quotef (name));

      return fd;
    }
  else
    {
      int fd_pair[2];
      pid_t child_pid;
      char const *shell_prog = getenv ("SHELL");
      if (shell_prog == NULL)
        shell_prog = "/bin/sh";
      if (setenv ("FILE", name, 1) != 0)
        die (EXIT_FAILURE, errno,
             _("failed to set FILE environment variable"));
      if (verbose)
        fprintf (stdout, _("executing with FILE=%s\n"), quotef (name));
      if (pipe (fd_pair) != 0)
        die (EXIT_FAILURE, errno, _("failed to create pipe"));
      child_pid = fork ();
      if (child_pid == 0)
        {
          /* This is the child process.  If an error occurs here, the
             parent will eventually learn about it after doing a wait,
             at which time it will emit its own error message.  */
          int j;
          /* We have to close any pipes that were opened during an
             earlier call, otherwise this process will be holding a
             write-pipe that will prevent the earlier process from
             reading an EOF on the corresponding read-pipe.  */
          for (j = 0; j < n_open_pipes; ++j)
            if (close (open_pipes[j]) != 0)
              die (EXIT_FAILURE, errno, _("closing prior pipe"));
          if (close (fd_pair[1]))
            die (EXIT_FAILURE, errno, _("closing output pipe"));
          if (fd_pair[0] != STDIN_FILENO)
            {
              if (dup2 (fd_pair[0], STDIN_FILENO) != STDIN_FILENO)
                die (EXIT_FAILURE, errno, _("moving input pipe"));
              if (close (fd_pair[0]) != 0)
                die (EXIT_FAILURE, errno, _("closing input pipe"));
            }
          sigprocmask (SIG_SETMASK, &oldblocked, NULL);
          execl (shell_prog, last_component (shell_prog), "-c",
                 filter_command, (char *) NULL);
          die (EXIT_FAILURE, errno, _("failed to run command: \"%s -c %s\""),
               shell_prog, filter_command);
        }
      if (child_pid == -1)
        die (EXIT_FAILURE, errno, _("fork system call failed"));
      if (close (fd_pair[0]) != 0)
        die (EXIT_FAILURE, errno, _("failed to close input pipe"));
      filter_pid = child_pid;
      if (n_open_pipes == open_pipes_alloc)
        open_pipes = x2nrealloc (open_pipes, &open_pipes_alloc,
                                 sizeof *open_pipes);
      open_pipes[n_open_pipes++] = fd_pair[1];
      return fd_pair[1];
    }
}

/* Close the output file, and do any associated cleanup.
   If FP and FD are both specified, they refer to the same open file;
   in this case FP is closed, but FD is still used in cleanup.  */
static void
closeout (FILE *fp, int fd, pid_t pid, char const *name)
{
  if (fp != NULL && fclose (fp) != 0 && ! ignorable (errno))
    die (EXIT_FAILURE, errno, "%s", quotef (name));
  if (fd >= 0)
    {
      if (fp == NULL && close (fd) < 0)
        die (EXIT_FAILURE, errno, "%s", quotef (name));
      int j;
      for (j = 0; j < n_open_pipes; ++j)
        {
          if (open_pipes[j] == fd)
            {
              open_pipes[j] = open_pipes[--n_open_pipes];
              break;
            }
        }
    }
  if (pid > 0)
    {
      int wstatus = 0;
      if (waitpid (pid, &wstatus, 0) == -1 && errno != ECHILD)
        die (EXIT_FAILURE, errno, _("waiting for child process"));
      if (WIFSIGNALED (wstatus))
        {
          int sig = WTERMSIG (wstatus);
          if (sig != SIGPIPE)
            {
              char signame[MAX (SIG2STR_MAX, INT_BUFSIZE_BOUND (int))];
              if (sig2str (sig, signame) != 0)
                sprintf (signame, "%d", sig);
              error (sig + 128, 0,
                     _("with FILE=%s, signal %s from command: %s"),
                     quotef (name), signame, filter_command);
            }
        }
      else if (WIFEXITED (wstatus))
        {
          int ex = WEXITSTATUS (wstatus);
          if (ex != 0)
            error (ex, 0, _("with FILE=%s, exit %d from command: %s"),
                   quotef (name), ex, filter_command);
        }
      else
        {
          /* shouldn't happen.  */
          die (EXIT_FAILURE, 0,
               _("unknown status from command (0x%X)"), wstatus + 0u);
        }
    }
}

/* Write BYTES bytes at BP to an output file.
   If NEW_FILE_FLAG is true, open the next output file.
   Otherwise add to the same output file already in use.
   Return true if successful.  */

static bool
cwrite (bool new_file_flag, const char *bp, size_t bytes)
{
  if (new_file_flag)
    {
      if (!bp && bytes == 0 && elide_empty_files)
        return true;
      closeout (NULL, output_desc, filter_pid, outfile);
      next_file_name ();
      output_desc = create (outfile);
      if (output_desc < 0)
        die (EXIT_FAILURE, errno, "%s", quotef (outfile));
    }

  if (full_write (output_desc, bp, bytes) == bytes)
    return true;
  else
    {
      if (! ignorable (errno))
        die (EXIT_FAILURE, errno, "%s", quotef (outfile));
      return false;
    }
}

/* Split into pieces of exactly N_BYTES bytes.
   Use buffer BUF, whose size is BUFSIZE.
   BUF contains the first INITIAL_READ input bytes.  */

static void
bytes_split (uintmax_t n_bytes, char *buf, size_t bufsize, size_t initial_read,
             uintmax_t max_files)
{
  size_t n_read;
  bool new_file_flag = true;
  uintmax_t to_write = n_bytes;
  uintmax_t opened = 0;
  bool eof;

  do
    {
      if (initial_read != SIZE_MAX)
        {
          n_read = initial_read;
          initial_read = SIZE_MAX;
          eof = n_read < bufsize;
        }
      else
        {
          n_read = safe_read (STDIN_FILENO, buf, bufsize);
          if (n_read == SAFE_READ_ERROR)
            die (EXIT_FAILURE, errno, "%s", quotef (infile));
          eof = n_read == 0;
        }
      char *bp_out = buf;
      size_t to_read = n_read;
      while (to_write <= to_read)
        {
          size_t w = to_write;
          bool cwrite_ok = cwrite (new_file_flag, bp_out, w);
          opened += new_file_flag;
          new_file_flag = !max_files || (opened < max_files);
          if (!new_file_flag && !cwrite_ok)
            {
              /* If filter no longer accepting input, stop reading.  */
              n_read = to_read = 0;
              break;
            }
          bp_out += w;
          to_read -= w;
          to_write = n_bytes;
        }
      if (to_read != 0)
        {
          bool cwrite_ok = cwrite (new_file_flag, bp_out, to_read);
          opened += new_file_flag;
          to_write -= to_read;
          new_file_flag = false;
          if (!cwrite_ok)
            {
              /* If filter no longer accepting input, stop reading.  */
              n_read = 0;
              break;
            }
        }
    }
  while (! eof);

  /* Ensure NUMBER files are created, which truncates
     any existing files or notifies any consumers on fifos.
     FIXME: Should we do this before EXIT_FAILURE?  */
  while (opened++ < max_files)
    cwrite (true, NULL, 0);
}

/* Split into pieces of exactly N_LINES lines.
   Use buffer BUF, whose size is BUFSIZE.  */

static void
lines_split (uintmax_t n_lines, char *buf, size_t bufsize)
{
  size_t n_read;
  char *bp, *bp_out, *eob;
  bool new_file_flag = true;
  uintmax_t n = 0;

  do
    {
      n_read = safe_read (STDIN_FILENO, buf, bufsize);
      if (n_read == SAFE_READ_ERROR)
        die (EXIT_FAILURE, errno, "%s", quotef (infile));
      bp = bp_out = buf;
      eob = bp + n_read;
      *eob = eolchar;
      while (true)
        {
          bp = memchr (bp, eolchar, eob - bp + 1);
          if (bp == eob)
            {
              if (eob != bp_out) /* do not write 0 bytes! */
                {
                  size_t len = eob - bp_out;
                  cwrite (new_file_flag, bp_out, len);
                  new_file_flag = false;
                }
              break;
            }

          ++bp;
          if (++n >= n_lines)
            {
              cwrite (new_file_flag, bp_out, bp - bp_out);
              bp_out = bp;
              new_file_flag = true;
              n = 0;
            }
        }
    }
  while (n_read);
}

/* Split into pieces that are as large as possible while still not more
   than N_BYTES bytes, and are split on line boundaries except
   where lines longer than N_BYTES bytes occur. */

static void
line_bytes_split (uintmax_t n_bytes, char *buf, size_t bufsize)
{
  size_t n_read;
  uintmax_t n_out = 0;      /* for each split.  */
  size_t n_hold = 0;
  char *hold = NULL;        /* for lines > bufsize.  */
  size_t hold_size = 0;
  bool split_line = false;  /* Whether a \n was output in a split.  */

  do
    {
      n_read = safe_read (STDIN_FILENO, buf, bufsize);
      if (n_read == SAFE_READ_ERROR)
        die (EXIT_FAILURE, errno, "%s", quotef (infile));
      size_t n_left = n_read;
      char *sob = buf;
      while (n_left)
        {
          size_t split_rest = 0;
          char *eoc = NULL;
          char *eol;

          /* Determine End Of Chunk and/or End of Line,
             which are used below to select what to write or buffer.  */
          if (n_bytes - n_out - n_hold <= n_left)
            {
              /* Have enough for split.  */
              split_rest = n_bytes - n_out - n_hold;
              eoc = sob + split_rest - 1;
              eol = memrchr (sob, eolchar, split_rest);
            }
          else
            eol = memrchr (sob, eolchar, n_left);

          /* Output hold space if possible.  */
          if (n_hold && !(!eol && n_out))
            {
              cwrite (n_out == 0, hold, n_hold);
              n_out += n_hold;
              if (n_hold > bufsize)
                hold = xrealloc (hold, bufsize);
              n_hold = 0;
              hold_size = bufsize;
            }

          /* Output to eol if present.  */
          if (eol)
            {
              split_line = true;
              size_t n_write = eol - sob + 1;
              cwrite (n_out == 0, sob, n_write);
              n_out += n_write;
              n_left -= n_write;
              sob += n_write;
              if (eoc)
                split_rest -= n_write;
            }

          /* Output to eoc or eob if possible.  */
          if (n_left && !split_line)
            {
              size_t n_write = eoc ? split_rest : n_left;
              cwrite (n_out == 0, sob, n_write);
              n_out += n_write;
              n_left -= n_write;
              sob += n_write;
              if (eoc)
                split_rest -= n_write;
            }

          /* Update hold if needed.  */
          if ((eoc && split_rest) || (!eoc && n_left))
            {
              size_t n_buf = eoc ? split_rest : n_left;
              if (hold_size - n_hold < n_buf)
                {
                  if (hold_size <= SIZE_MAX - bufsize)
                    hold_size += bufsize;
                  else
                    xalloc_die ();
                  hold = xrealloc (hold, hold_size);
                }
              memcpy (hold + n_hold, sob, n_buf);
              n_hold += n_buf;
              n_left -= n_buf;
              sob += n_buf;
            }

          /* Reset for new split.  */
          if (eoc)
            {
              n_out = 0;
              split_line = false;
            }
        }
    }
  while (n_read);

  /* Handle no eol at end of file.  */
  if (n_hold)
    cwrite (n_out == 0, hold, n_hold);

  free (hold);
}

/* -n l/[K/]N: Write lines to files of approximately file size / N.
   The file is partitioned into file size / N sized portions, with the
   last assigned any excess.  If a line _starts_ within a partition
   it is written completely to the corresponding file.  Since lines
   are not split even if they overlap a partition, the files written
   can be larger or smaller than the partition size, and even empty
   if a line is so long as to completely overlap the partition.  */

static void
lines_chunk_split (uintmax_t k, uintmax_t n, char *buf, size_t bufsize,
                   size_t initial_read, off_t file_size)
{
  assert (n && k <= n && n <= file_size);

  const off_t chunk_size = file_size / n;
  uintmax_t chunk_no = 1;
  off_t chunk_end = chunk_size - 1;
  off_t n_written = 0;
  bool new_file_flag = true;
  bool chunk_truncated = false;

  if (k > 1)
    {
      /* Start reading 1 byte before kth chunk of file.  */
      off_t start = (k - 1) * chunk_size - 1;
      if (start < initial_read)
        {
          memmove (buf, buf + start, initial_read - start);
          initial_read -= start;
        }
      else
        {
          if (lseek (STDIN_FILENO, start - initial_read, SEEK_CUR) < 0)
            die (EXIT_FAILURE, errno, "%s", quotef (infile));
          initial_read = SIZE_MAX;
        }
      n_written = start;
      chunk_no = k - 1;
      chunk_end = chunk_no * chunk_size - 1;
    }

  while (n_written < file_size)
    {
      char *bp = buf, *eob;
      size_t n_read;
      if (initial_read != SIZE_MAX)
        {
          n_read = initial_read;
          initial_read = SIZE_MAX;
        }
      else
        {
          n_read = safe_read (STDIN_FILENO, buf, bufsize);
          if (n_read == SAFE_READ_ERROR)
            die (EXIT_FAILURE, errno, "%s", quotef (infile));
        }
      if (n_read == 0)
        break; /* eof.  */
      n_read = MIN (n_read, file_size - n_written);
      chunk_truncated = false;
      eob = buf + n_read;

      while (bp != eob)
        {
          size_t to_write;
          bool next = false;

          /* Begin looking for '\n' at last byte of chunk.  */
          off_t skip = MIN (n_read, MAX (0, chunk_end - n_written));
          char *bp_out = memchr (bp + skip, eolchar, n_read - skip);
          if (bp_out++)
            next = true;
          else
            bp_out = eob;
          to_write = bp_out - bp;

          if (k == chunk_no)
            {
              /* We don't use the stdout buffer here since we're writing
                 large chunks from an existing file, so it's more efficient
                 to write out directly.  */
              if (full_write (STDOUT_FILENO, bp, to_write) != to_write)
                die (EXIT_FAILURE, errno, "%s", _("write error"));
            }
          else if (! k)
            cwrite (new_file_flag, bp, to_write);
          n_written += to_write;
          bp += to_write;
          n_read -= to_write;
          new_file_flag = next;

          /* A line could have been so long that it skipped
             entire chunks. So create empty files in that case.  */
          while (next || chunk_end <= n_written - 1)
            {
              if (!next && bp == eob)
                {
                  /* replenish buf, before going to next chunk.  */
                  chunk_truncated = true;
                  break;
                }
              chunk_no++;
              if (k && chunk_no > k)
                return;
              if (chunk_no == n)
                chunk_end = file_size - 1; /* >= chunk_size.  */
              else
                chunk_end += chunk_size;
              if (chunk_end <= n_written - 1)
                {
                  if (! k)
                    cwrite (true, NULL, 0);
                }
              else
                next = false;
            }
        }
    }

  if (chunk_truncated)
    chunk_no++;

  /* Ensure NUMBER files are created, which truncates
     any existing files or notifies any consumers on fifos.
     FIXME: Should we do this before EXIT_FAILURE?  */
  while (!k && chunk_no++ <= n)
    cwrite (true, NULL, 0);
}

/* -n K/N: Extract Kth of N chunks.  */

static void
bytes_chunk_extract (uintmax_t k, uintmax_t n, char *buf, size_t bufsize,
                     size_t initial_read, off_t file_size)
{
  off_t start;
  off_t end;

  assert (k && n && k <= n && n <= file_size);

  start = (k - 1) * (file_size / n);
  end = (k == n) ? file_size : k * (file_size / n);

if(__cpr_choice("L290", "bool", (long long[]){start, initial_read, bufsize}, (char*[]){"start","initial_read", "bufsize"}, 3, (int*[]){}, (char*[]){}, 0))
    {


assert(initial_read > start);
      memmove (buf, buf + start, initial_read - start);
      initial_read -= start;
    }
  else
    {
      if (lseek (STDIN_FILENO, start, SEEK_CUR) < 0)
        die (EXIT_FAILURE, errno, "%s", quotef (infile));
      initial_read = SIZE_MAX;
    }

  while (start < end)
    {
      size_t n_read;
      if (initial_read != SIZE_MAX)
        {
          n_read = initial_read;
          initial_read = SIZE_MAX;
        }
      else
        {
          n_read = safe_read (STDIN_FILENO, buf, bufsize);
          if (n_read == SAFE_READ_ERROR)
            die (EXIT_FAILURE, errno, "%s", quotef (infile));
        }
      if (n_read == 0)
        break; /* eof.  */
      n_read = MIN (n_read, end - start);
      if (full_write (STDOUT_FILENO, buf, n_read) != n_read
          && ! ignorable (errno))
        die (EXIT_FAILURE, errno, "%s", quotef ("-"));
      start += n_read;
    }
}

typedef struct of_info
{
  char *of_name;
  int ofd;
  FILE *ofile;
  int opid;
} of_t;

enum
{
  OFD_NEW = -1,
  OFD_APPEND = -2
};

/* Rotate file descriptors when we're writing to more output files than we
   have available file descriptors.
   Return whether we came under file resource pressure.
   If so, it's probably best to close each file when finished with it.  */

static bool
ofile_open (of_t *files, size_t i_check, size_t nfiles)
{
  bool file_limit = false;

  if (files[i_check].ofd <= OFD_NEW)
    {
      int fd;
      size_t i_reopen = i_check ? i_check - 1 : nfiles - 1;

      /* Another process could have opened a file in between the calls to
         close and open, so we should keep trying until open succeeds or
         we've closed all of our files.  */
      while (true)
        {
          if (files[i_check].ofd == OFD_NEW)
            fd = create (files[i_check].of_name);
          else /* OFD_APPEND  */
            {
              /* Attempt to append to previously opened file.
                 We use O_NONBLOCK to support writing to fifos,
                 where the other end has closed because of our
                 previous close.  In that case we'll immediately
                 get an error, rather than waiting indefinitely.
                 In specialised cases the consumer can keep reading
                 from the fifo, terminating on conditions in the data
                 itself, or perhaps never in the case of 'tail -f'.
                 I.e., for fifos it is valid to attempt this reopen.

                 We don't handle the filter_command case here, as create()
                 will exit if there are not enough files in that case.
                 I.e., we don't support restarting filters, as that would
                 put too much burden on users specifying --filter commands.  */
              fd = open (files[i_check].of_name,
                         O_WRONLY | O_BINARY | O_APPEND | O_NONBLOCK);
            }

          if (-1 < fd)
            break;

          if (!(errno == EMFILE || errno == ENFILE))
            die (EXIT_FAILURE, errno, "%s", quotef (files[i_check].of_name));

          file_limit = true;

          /* Search backwards for an open file to close.  */
          while (files[i_reopen].ofd < 0)
            {
              i_reopen = i_reopen ? i_reopen - 1 : nfiles - 1;
              /* No more open files to close, exit with E[NM]FILE.  */
              if (i_reopen == i_check)
                die (EXIT_FAILURE, errno, "%s",
                     quotef (files[i_check].of_name));
            }

          if (fclose (files[i_reopen].ofile) != 0)
            die (EXIT_FAILURE, errno, "%s", quotef (files[i_reopen].of_name));
          files[i_reopen].ofile = NULL;
          files[i_reopen].ofd = OFD_APPEND;
        }

      files[i_check].ofd = fd;
      if (!(files[i_check].ofile = fdopen (fd, "a")))
        die (EXIT_FAILURE, errno, "%s", quotef (files[i_check].of_name));
      files[i_check].opid = filter_pid;
      filter_pid = 0;
    }

  return file_limit;
}

/* -n r/[K/]N: Divide file into N chunks in round robin fashion.
   When K == 0, we try to keep the files open in parallel.
   If we run out of file resources, then we revert
   to opening and closing each file for each line.  */

static void
lines_rr (uintmax_t k, uintmax_t n, char *buf, size_t bufsize)
{
  bool wrapped = false;
  bool wrote = false;
  bool file_limit;
  size_t i_file;
  of_t *files IF_LINT (= NULL);
  uintmax_t line_no;

  if (k)
    line_no = 1;
  else
    {
      if (SIZE_MAX < n)
        xalloc_die ();
      files = xnmalloc (n, sizeof *files);

      /* Generate output file names. */
      for (i_file = 0; i_file < n; i_file++)
        {
          next_file_name ();
          files[i_file].of_name = xstrdup (outfile);
          files[i_file].ofd = OFD_NEW;
          files[i_file].ofile = NULL;
          files[i_file].opid = 0;
        }
      i_file = 0;
      file_limit = false;
    }

  while (true)
    {
      char *bp = buf, *eob;
      size_t n_read = safe_read (STDIN_FILENO, buf, bufsize);
      if (n_read == SAFE_READ_ERROR)
        die (EXIT_FAILURE, errno, "%s", quotef (infile));
      else if (n_read == 0)
        break; /* eof.  */
      eob = buf + n_read;

      while (bp != eob)
        {
          size_t to_write;
          bool next = false;

          /* Find end of line. */
          char *bp_out = memchr (bp, eolchar, eob - bp);
          if (bp_out)
            {
              bp_out++;
              next = true;
            }
          else
            bp_out = eob;
          to_write = bp_out - bp;

          if (k)
            {
              if (line_no == k && unbuffered)
                {
                  if (full_write (STDOUT_FILENO, bp, to_write) != to_write)
                    die (EXIT_FAILURE, errno, "%s", _("write error"));
                }
              else if (line_no == k && fwrite (bp, to_write, 1, stdout) != 1)
                {
                  clearerr (stdout); /* To silence close_stdout().  */
                  die (EXIT_FAILURE, errno, "%s", _("write error"));
                }
              if (next)
                line_no = (line_no == n) ? 1 : line_no + 1;
            }
          else
            {
              /* Secure file descriptor. */
              file_limit |= ofile_open (files, i_file, n);
              if (unbuffered)
                {
                  /* Note writing to fd, rather than flushing the FILE gives
                     an 8% performance benefit, due to reduced data copying.  */
                  if (full_write (files[i_file].ofd, bp, to_write) != to_write
                      && ! ignorable (errno))
                    {
                      die (EXIT_FAILURE, errno, "%s",
                           quotef (files[i_file].of_name));
                    }
                }
              else if (fwrite (bp, to_write, 1, files[i_file].ofile) != 1
                       && ! ignorable (errno))
                {
                  die (EXIT_FAILURE, errno, "%s",
                       quotef (files[i_file].of_name));
                }

              if (! ignorable (errno))
                wrote = true;

              if (file_limit)
                {
                  if (fclose (files[i_file].ofile) != 0)
                    {
                      die (EXIT_FAILURE, errno, "%s",
                           quotef (files[i_file].of_name));
                    }
                  files[i_file].ofile = NULL;
                  files[i_file].ofd = OFD_APPEND;
                }
              if (next && ++i_file == n)
                {
                  wrapped = true;
                  /* If no filters are accepting input, stop reading.  */
                  if (! wrote)
                    goto no_filters;
                  wrote = false;
                  i_file = 0;
                }
            }

          bp = bp_out;
        }
    }

no_filters:
  /* Ensure all files created, so that any existing files are truncated,
     and to signal any waiting fifo consumers.
     Also, close any open file descriptors.
     FIXME: Should we do this before EXIT_FAILURE?  */
  if (!k)
    {
      int ceiling = (wrapped ? n : i_file);
      for (i_file = 0; i_file < n; i_file++)
        {
          if (i_file >= ceiling && !elide_empty_files)
            file_limit |= ofile_open (files, i_file, n);
          if (files[i_file].ofd >= 0)
            closeout (files[i_file].ofile, files[i_file].ofd,
                      files[i_file].opid, files[i_file].of_name);
          files[i_file].ofd = OFD_APPEND;
        }
    }
  IF_LINT (free (files));
}

#define FAIL_ONLY_ONE_WAY()					\
  do								\
    {								\
      error (0, 0, _("cannot split in more than one way"));	\
      usage (EXIT_FAILURE);					\
    }								\
  while (0)


/* Parse K/N syntax of chunk options.  */

static void
parse_chunk (uintmax_t *k_units, uintmax_t *n_units, char *slash)
{
  *n_units = xdectoumax (slash + 1, 1, UINTMAX_MAX, "",
                         _("invalid number of chunks"), 0);
  if (slash != optarg)           /* a leading number is specified.  */
    {
      *slash = '\0';
      *k_units = xdectoumax (optarg, 1, *n_units, "",
                             _("invalid chunk number"), 0);
    }
}


int
main (int argc, char **argv)
{
  enum Split_type split_type = type_undef;
  size_t in_blk_size = 0;	/* optimal block size of input file device */
  size_t page_size = getpagesize ();
  uintmax_t k_units = 0;
  uintmax_t n_units = 0;

  static char const multipliers[] = "bEGKkMmPTYZ0";
  int c;
  int digits_optind = 0;
  off_t file_size = OFF_T_MAX;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* Parse command line options.  */

  infile = bad_cast ("-");
  outbase = bad_cast ("x");

  while (true)
    {
      /* This is the argv-index of the option we will read next.  */
      int this_optind = optind ? optind : 1;
      char *slash;

      c = getopt_long (argc, argv, "0123456789C:a:b:del:n:t:u",
                       longopts, NULL);
      if (c == -1)
        break;

      switch (c)
        {
        case 'a':
          suffix_length = xdectoumax (optarg, 0, SIZE_MAX / sizeof (size_t),
                                      "", _("invalid suffix length"), 0);
          break;

        case ADDITIONAL_SUFFIX_OPTION:
          if (last_component (optarg) != optarg)
            {
              error (0, 0,
                     _("invalid suffix %s, contains directory separator"),
                     quote (optarg));
              usage (EXIT_FAILURE);
            }
          additional_suffix = optarg;
          break;

        case 'b':
          if (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          split_type = type_bytes;
          /* Limit to OFF_T_MAX, because if input is a pipe, we could get more
             data than is possible to write to a single file, so indicate that
             immediately rather than having possibly future invocations fail. */
          n_units = xdectoumax (optarg, 1, OFF_T_MAX, multipliers,
                                _("invalid number of bytes"), 0);
          break;

        case 'l':
          if (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          split_type = type_lines;
          n_units = xdectoumax (optarg, 1, UINTMAX_MAX, "",
                                _("invalid number of lines"), 0);
          break;

        case 'C':
          if (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          split_type = type_byteslines;
          n_units = xdectoumax (optarg, 1, MIN (SIZE_MAX, OFF_T_MAX),
                                multipliers, _("invalid number of bytes"), 0);
          break;

        case 'n':
          if (split_type != type_undef)
            FAIL_ONLY_ONE_WAY ();
          /* skip any whitespace */
          while (isspace (to_uchar (*optarg)))
            optarg++;
          if (STRNCMP_LIT (optarg, "r/") == 0)
            {
              split_type = type_rr;
              optarg += 2;
            }
          else if (STRNCMP_LIT (optarg, "l/") == 0)
            {
              split_type = type_chunk_lines;
              optarg += 2;
            }
          else
            split_type = type_chunk_bytes;
          if ((slash = strchr (optarg, '/')))
            parse_chunk (&k_units, &n_units, slash);
          else
            n_units = xdectoumax (optarg, 1, UINTMAX_MAX, "",
                                  _("invalid number of chunks"), 0);
          break;

        case 'u':
          unbuffered = true;
          break;

        case 't':
          {
            char neweol = optarg[0];
            if (! neweol)
              die (EXIT_FAILURE, 0, _("empty record separator"));
            if (optarg[1])
              {
                if (STREQ (optarg, "\\0"))
                  neweol = '\0';
                else
                  {
                    /* Provoke with 'split -txx'.  Complain about
                       "multi-character tab" instead of "multibyte tab", so
                       that the diagnostic's wording does not need to be
                       changed once multibyte characters are supported.  */
                    die (EXIT_FAILURE, 0, _("multi-character separator %s"),
                         quote (optarg));
                  }
              }
            /* Make it explicit we don't support multiple separators.  */
            if (0 <= eolchar && neweol != eolchar)
              {
                die (EXIT_FAILURE, 0,
                     _("multiple separator characters specified"));
              }

            eolchar = neweol;
          }
          break;

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
          if (split_type == type_undef)
            {
              split_type = type_digits;
              n_units = 0;
            }
          if (split_type != type_undef && split_type != type_digits)
            FAIL_ONLY_ONE_WAY ();
          if (digits_optind != 0 && digits_optind != this_optind)
            n_units = 0;	/* More than one number given; ignore other. */
          digits_optind = this_optind;
          if (!DECIMAL_DIGIT_ACCUMULATE (n_units, c - '0', uintmax_t))
            {
              char buffer[INT_BUFSIZE_BOUND (uintmax_t)];
              die (EXIT_FAILURE, 0,
                   _("line count option -%s%c... is too large"),
                   umaxtostr (n_units, buffer), c);
            }
          break;

        case 'd':
          suffix_alphabet = "0123456789";
          if (optarg)
            {
              if (strlen (optarg) != strspn (optarg, suffix_alphabet))
                {
                  error (0, 0,
                         _("%s: invalid start value for numerical suffix"),
                         quote (optarg));
                  usage (EXIT_FAILURE);
                }
              else
                {
                  /* Skip any leading zero.  */
                  while (*optarg == '0' && *(optarg + 1) != '\0')
                    optarg++;
                  numeric_suffix_start = optarg;
                }
            }
          break;

        case 'e':
          elide_empty_files = true;
          break;

        case FILTER_OPTION:
          filter_command = optarg;
          break;

        case IO_BLKSIZE_OPTION:
          in_blk_size = xdectoumax (optarg, 1, SIZE_MAX - page_size,
                                    multipliers, _("invalid IO block size"), 0);
          break;

        case VERBOSE_OPTION:
          verbose = true;
          break;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        default:
          usage (EXIT_FAILURE);
        }
    }

  if (k_units != 0 && filter_command)
    {
      error (0, 0, _("--filter does not process a chunk extracted to stdout"));
      usage (EXIT_FAILURE);
    }

  /* Handle default case.  */
  if (split_type == type_undef)
    {
      split_type = type_lines;
      n_units = 1000;
    }

  if (n_units == 0)
    {
      error (0, 0, "%s: %s", _("invalid number of lines"), quote ("0"));
      usage (EXIT_FAILURE);
    }

  if (eolchar < 0)
    eolchar = '\n';

  set_suffix_length (n_units, split_type);

  /* Get out the filename arguments.  */

  if (optind < argc)
    infile = argv[optind++];

  if (optind < argc)
    outbase = argv[optind++];

  if (optind < argc)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  /* Check that the suffix length is large enough for the numerical
     suffix start value.  */
  if (numeric_suffix_start && strlen (numeric_suffix_start) > suffix_length)
    {
      error (0, 0, _("numerical suffix start value is too large "
                     "for the suffix length"));
      usage (EXIT_FAILURE);
    }

  /* Open the input file.  */
  if (! STREQ (infile, "-")
      && fd_reopen (STDIN_FILENO, infile, O_RDONLY, 0) < 0)
    die (EXIT_FAILURE, errno, _("cannot open %s for reading"),
         quoteaf (infile));

  /* Binary I/O is safer when byte counts are used.  */
  if (O_BINARY && ! isatty (STDIN_FILENO))
    xfreopen (NULL, "rb", stdin);

  /* Get the optimal block size of input device and make a buffer.  */

  if (fstat (STDIN_FILENO, &in_stat_buf) != 0)
    die (EXIT_FAILURE, errno, "%s", quotef (infile));

  bool specified_buf_size = !! in_blk_size;
  if (! specified_buf_size)
    in_blk_size = io_blksize (in_stat_buf);

  void *b = xmalloc (in_blk_size + 1 + page_size - 1);
  char *buf = ptr_align (b, page_size);
  size_t initial_read = SIZE_MAX;

  if (split_type == type_chunk_bytes || split_type == type_chunk_lines)
    {
      file_size = input_file_size (STDIN_FILENO, &in_stat_buf,
                                   buf, in_blk_size);
      if (file_size < 0)
        die (EXIT_FAILURE, errno, _("%s: cannot determine file size"),
             quotef (infile));
      initial_read = MIN (file_size, in_blk_size);
      /* Overflow, and sanity checking.  */
      if (OFF_T_MAX < n_units)
        {
          char buffer[INT_BUFSIZE_BOUND (uintmax_t)];
          die (EXIT_FAILURE, EOVERFLOW, "%s: %s",
               _("invalid number of chunks"),
               quote (umaxtostr (n_units, buffer)));
        }
      /* increase file_size to n_units here, so that we still process
         any input data, and create empty files for the rest.  */
      file_size = MAX (file_size, n_units);
    }

  /* When filtering, closure of one pipe must not terminate the process,
     as there may still be other streams expecting input from us.  */
  if (filter_command)
    {
      struct sigaction act;
      sigemptyset (&newblocked);
      sigaction (SIGPIPE, NULL, &act);
      if (act.sa_handler != SIG_IGN)
        sigaddset (&newblocked, SIGPIPE);
      sigprocmask (SIG_BLOCK, &newblocked, &oldblocked);
    }

  switch (split_type)
    {
    case type_digits:
    case type_lines:
      lines_split (n_units, buf, in_blk_size);
      break;

    case type_bytes:
      bytes_split (n_units, buf, in_blk_size, SIZE_MAX, 0);
      break;

    case type_byteslines:
      line_bytes_split (n_units, buf, in_blk_size);
      break;

    case type_chunk_bytes:
      if (k_units == 0)
        bytes_split (file_size / n_units, buf, in_blk_size, initial_read,
                     n_units);
      else
        bytes_chunk_extract (k_units, n_units, buf, in_blk_size, initial_read,
                             file_size);
      break;

    case type_chunk_lines:
      lines_chunk_split (k_units, n_units, buf, in_blk_size, initial_read,
                         file_size);
      break;

    case type_rr:
      /* Note, this is like 'sed -n ${k}~${n}p' when k > 0,
         but the functionality is provided for symmetry.  */
      lines_rr (k_units, n_units, buf, in_blk_size);
      break;

    default:
      abort ();
    }

  IF_LINT (free (b));

  if (close (STDIN_FILENO) != 0)
    die (EXIT_FAILURE, errno, "%s", quotef (infile));
  closeout (NULL, output_desc, filter_pid, outfile);

  return EXIT_SUCCESS;
}
