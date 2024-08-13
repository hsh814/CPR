#!/bin/bash
rm -rf src
rm -rf patched && mkdir patched
git clone https://github.com/coreutils/coreutils.git src
pushd src
  git checkout 658529a
  git clone https://github.com/coreutils/gnulib.git
  ./bootstrap
  # patch
  cp ../make-prime-list.c src/make-prime-list.c
  FORCE_UNSAFE_CONFIGURE=1 CC=cpr-cc CXX=cpr-cxx ./configure CFLAGS='-g -O0 -static -fPIE' CXXFLAGS="$CFLAGS"
  make -j 32
  # cp
  cp src/make-prime-list.c ../patched
  cp src/make-prime-list ../patched
popd
pushd patched
  extract-bc make-prime-list
popd
