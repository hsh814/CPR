#!/bin/bash
rm -rf san2patch-src
rm -rf san2patch-patched && mkdir san2patch-patched
git clone https://github.com/coreutils/coreutils.git san2patch-src
pushd san2patch-src
  git checkout 658529a
  git clone https://github.com/coreutils/gnulib.git
  ./bootstrap
  # patch
  cp ../make-prime-list.san2patch-cpr.c src/make-prime-list.c
  FORCE_UNSAFE_CONFIGURE=1 LD=lld LD=lld CC=wllvm CXX=wllvm++ ./configure CFLAGS='-g -O0 -fno-discard-value-names -static -fPIE' CXXFLAGS="$CFLAGS"
  make CFLAGS="-fno-discard-value-names -fPIC -fPIE -L/root/projects/uni-klee/build/lib  -lkleeRuntest" CXXFLAGS=$CFLAGS src/make-prime-list -j32
  # cp
  cp src/make-prime-list.c ../san2patch-patched
  cp src/make-prime-list ../san2patch-patched
popd
pushd san2patch-patched
  extract-bc make-prime-list
popd
