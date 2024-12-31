#!/bin/bash
CPR_DIR=${CPR_DIR:-/root/projects/CPR}
PATH=$PATH:$CPR_DIR/tools
LIB_DIR=$CPR_DIR/lib
rm -rf val-src val-runtime
mkdir -p val-runtime
project_url=https://github.com/coreutils/coreutils.git
commit_id=ca99c52
patched_dir=src
patched_file=pr.c
bin_dir=src
bin_file=pr
git clone $project_url val-src
pushd val-src
  git checkout $commit_id
  git clone https://github.com/coreutils/gnulib.git
  ./bootstrap
  # Patch
  cp ../pr.val.c ${patched_dir}/${patched_file}
  rm -rf build
  mkdir build
  pushd build
    FORCE_UNSAFE_CONFIGURE=1 CC=clang CXX=clang++ ../configure CFLAGS="-Wno-error -fsanitize=address -fsanitize=undefined -g -L$LIB_DIR -luni_klee_memory_check" CXXFLAGS="-Wno-error -fsanitize=address -fsanitize=undefined -g -L$LIB_DIR -luni_klee_memory_check"
    make  -j32
  popd
  # cp
  cp ${patched_dir}/${patched_file} ../val-runtime
  cp build/${bin_dir}/${bin_file} ../val-runtime
popd
