#!/bin/bash
CPR_DIR=${CPR_DIR:-/root/projects/CPR}
PATH=$PATH:$CPR_DIR/tools
rm -rf src patched
mkdir -p patched
project_url=https://github.com/vadz/libtiff.git
commit_id=f3069a5
patched_dir=tools
patched_file=tiff2ps.c
mode_name="extractfix"
patched_file_from=${patched_file%.c}.${mode_name}.c
bin_dir=tools
bin_file=tiff2ps
git clone $project_url src
pushd src
  git checkout $commit_id
  # Patch
  cp ../${patched_file_from} ${patched_dir}/${patched_file}
  # Remove longjmp calls
  sed -i '118d;221d' libtiff/tif_jpeg.c
  sed -i '153d;2463d' libtiff/tif_ojpeg.c
  ./autogen.sh
  LD=lld CC=cpr-cc CXX=cpr-cxx ./configure --enable-static --disable-shared --without-threads --without-lzma
  CC=cpr-cc CXX=cpr-cxx make CFLAGS="-static -O0 -g -fno-discard-value-names" CXXFLAGS="-static -O0 -g -fno-discard-value-names" -j16
  # cp
  cp ${patched_dir}/${patched_file} ../patched
  cp ${bin_dir}/${bin_file} ../patched
popd
pushd patched
  cp ../exploit.tif .
  extract-bc ${bin_file}
popd
