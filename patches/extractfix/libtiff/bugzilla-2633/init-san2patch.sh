#!/bin/bash
CPR_DIR=${CPR_DIR:-/root/projects/CPR}
PATH=$PATH:$CPR_DIR/tools
rm -rf san2patch-src san2patch-patched
mkdir -p san2patch-patched
project_url=https://github.com/vadz/libtiff.git
commit_id=f3069a5
patched_dir=tools
patched_file=tiff2ps.c
bin_dir=tools
bin_file=tiff2ps
git clone $project_url san2patch-src
pushd san2patch-src
  git checkout $commit_id
  # Patch
  cp "../${patched_file%.c}.san2patch-cpr.c" ${patched_dir}/${patched_file}
  # Remove longjmp calls
  sed -i '118d;221d' libtiff/tif_jpeg.c
  sed -i '153d;2463d' libtiff/tif_ojpeg.c
  ./autogen.sh
  LD=lld CC=wllvm CXX=wllvm++ ./configure --enable-static --disable-shared --without-threads --without-lzma
  CC=wllvm CXX=wllvm++ make CFLAGS="-static -O0 -g -fno-discard-value-names" CXXFLAGS="-static -O0 -g -fno-discard-value-names" -j16
  # cp
  cp ${patched_dir}/${patched_file} ../san2patch-patched
  cp ${bin_dir}/${bin_file} ../san2patch-patched
popd
pushd san2patch-patched
  cp ../exploit.tif .
  extract-bc ${bin_file}
popd
