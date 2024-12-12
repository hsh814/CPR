#!/bin/bash
CPR_DIR=${CPR_DIR:-/root/projects/CPR}
PATH=$PATH:$CPR_DIR/tools
LIB_DIR=$CPR_DIR/lib
rm -rf val-src val-runtime
mkdir -p val-runtime
project_url=https://github.com/vadz/libtiff.git
commit_id=f3069a5
patched_dir=tools
patched_file=tiff2ps.c
bin_dir=tools
bin_file=tiff2ps
git clone $project_url val-src
pushd val-src
  git checkout $commit_id
  # Patch
  cp ../tiff2ps.val.c ${patched_dir}/${patched_file}
  ./autogen.sh
  LD=lld CC=clang CXX=clang++ ./configure --enable-static --disable-shared --without-threads --without-lzma
  CC=clang CXX=clang++ make CFLAGS="-static -O0 -g -fno-discard-value-names -L$LIB_DIR -luni_klee_memory_check" CXXFLAGS="-static -O0 -g -fno-discard-value-names -L$LIB_DIR -luni_klee_memory_check" -j16
  # cp
  cp ${patched_dir}/${patched_file} ../val-runtime
  cp ${bin_dir}/${bin_file} ../val-runtime
popd
