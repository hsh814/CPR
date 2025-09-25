#!/bin/bash
CPR_DIR=${CPR_DIR:-/root/projects/CPR}
PATH=$PATH:$CPR_DIR/tools
rm -rf dafl-src dafl-patched
mkdir -p dafl-patched
project_url=https://github.com/coreutils/coreutils.git
commit_id=68c5eec
patched_dir=src
patched_file=split.c
bin_dir=src
bin_file=split
git clone $project_url dafl-src
pushd dafl-src
  git checkout $commit_id
  git clone https://github.com/coreutils/gnulib.git
  # Build
  ./bootstrap
    # Patch
  cp "../${patched_file%.c}.afl.c" ${patched_dir}/${patched_file}
  rm -rf build
  mkdir build
  pushd build
    CC=clang CXX=clang++ FORCE_UNSAFE_CONFIGURE=1 ../configure
    CC=clang CXX=clang++ make LDFLAGS="-no-pie" CFLAGS="-Wno-error -fsanitize=address -g -fPIC" CXXFLAGS="-Wno-error -fsanitize=address -g -fPIC" -j 10
  popd
  # cp
  cp build/${bin_dir}/${bin_file} ../dafl-patched/bin
popd