#!/bin/bash
CPR_DIR=${CPR_DIR:-/root/projects/CPR}
PATH=$PATH:$CPR_DIR/tools
rm -rf san2patch-src san2patch-patched
mkdir -p san2patch-patched
project_url=https://github.com/coreutils/coreutils.git
commit_id=68c5eec
patched_dir=src
patched_file=split.c
bin_dir=src
bin_file=split
git clone $project_url san2patch-src
pushd san2patch-src
  git checkout $commit_id
  git clone https://github.com/coreutils/gnulib.git
  # Build
  ./bootstrap
    # Patch
  cp "../${patched_file%.c}.san2patch-cpr.c" ${patched_dir}/${patched_file}
  rm -rf build
  mkdir build
  pushd build
    FORCE_UNSAFE_CONFIGURE=1 LD=lld CC=wllvm CXX=wllvm++ ../configure CFLAGS='-g -O0 -fno-discard-value-names -static -fPIE -fPIC' CXXFLAGS="$CFLAGS"
    make CFLAGS="-fno-discard-value-names -fPIC -fPIE -L/root/projects/uni-klee/build/lib  -lkleeRuntest -I/root/projects/uni-klee/include" CXXFLAGS=$CFLAGS -j32
  popd
  # cp
  cp ${patched_dir}/${patched_file} ../san2patch-patched
  cp build/${bin_dir}/${bin_file} ../san2patch-patched
popd
pushd san2patch-patched
  cp ../exploit.txt .
  extract-bc ${bin_file}
popd
