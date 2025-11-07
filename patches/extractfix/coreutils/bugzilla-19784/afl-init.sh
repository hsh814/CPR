#!/bin/bash
rm -rf dafl-src
rm -rf dafl-patched && mkdir dafl-patched
git clone https://github.com/coreutils/coreutils.git dafl-src
pushd dafl-src
  git checkout 658529a
  git clone https://github.com/coreutils/gnulib.git
  ./bootstrap
  # patch
  cp ../make-prime-list.afl.c src/make-prime-list.c
  CC=clang CXX=clang++ FORCE_UNSAFE_CONFIGURE=1 ./configure
  CC=clang CXX=clang++ make LDFLAGS="-no-pie" CFLAGS="-Wno-error -fsanitize=address -g -fPIC" src/make-prime-list
  # cp
  cp src/make-prime-list ../dafl-patched/bin
popd
