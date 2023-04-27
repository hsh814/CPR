#!/bin/bash
CPR_DIR=${CPR_DIR:-/root/projects/CPR}
PATH=$PATH:$CPR_DIR/tools
rm -rf src patched
mkdir -p patched
project_url=https://github.com/vadz/libtiff.git
commit_id=9a72a69
patched_dir=libtiff
patched_file=tif_ojpeg.c
bin_dir=tools
bin_file=tiffmedian
git clone $project_url src
pushd src
  git checkout $commit_id
  # Patch
  cp ../${patched_file} ${target_dir}/${patched_file}
  ./autogen.sh
  CC=cpr-cc CXX=cpr-cxx ./configure --enable-static --disable-shared --without-threads --without-lzma
  CC=cpr-cc CXX=cpr-cxx make CFLAGS="-static -O0 -g" CXXFLAGS="-static -O0 -g" -j16
  # cp
  cp ${target_dir}/${patched_file} ../patched
  cp ${bin_dir}/${bin_file} ../patched
popd
pushd patched
  cp ../exploit.tif .
  extract-bc ${bin_file}
popd
