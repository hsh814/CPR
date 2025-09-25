#!/bin/bash
CPR_DIR=${CPR_DIR:-/root/projects/CPR}
PATH=$PATH:$CPR_DIR/tools
rm -rf dafl-src dafl-patched
mkdir -p dafl-patched
project_url=https://github.com/vadz/libtiff.git
commit_id=9a72a69
patched_dir=libtiff
patched_file=tif_ojpeg.c
bin_dir=tools
bin_file=tiffmedian
git clone $project_url dafl-src
pushd dafl-src
  git checkout $commit_id
  wget http://www.ijg.org/files/jpegsrc.v8d.tar.gz
  tar xvzf jpegsrc.v8d.tar.gz
  pushd jpeg-8d
    ./configure --prefix=${PWD}/build
    make -j32 install
  popd
  # Patch
  cp "../${patched_file%.c}.afl.c" ${patched_dir}/${patched_file}
  ./autogen.sh
  CC=clang CXX=clang++ OJPEG_SUPPORT=true JPEG_SUPPORT=true ./configure --enable-static --disable-shared --enable-old-jpeg --with-jpeg-include-dir="${PWD}/jpeg-8d/build/include" --with-jpeg-lib-dir="${PWD}/jpeg-8d/build/lib"
  CC=clang CXX=clang++ OJPEG_SUPPORT=true JPEG_SUPPORT=true make CFLAGS="-static -fsanitize=address -fsanitize=undefined -g" CXXFLAGS="-static -fsanitize=address -fsanitize=undefined -g" -j10
  # cp
  cp ${bin_dir}/${bin_file} ../dafl-patched/bin
popd