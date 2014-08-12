#!/bin/bash
DIR="$(dirname $0)"

cd ./third_party
git submodule update --init --recursive

# configure libevhtp
cd ./libevhtp/build
cmake .. -DCMAKE_INSTALL_PREFIX:PATH=$DIR/../../third_party
cd ../../

# configure ctemplate
cd ./ctemplate-read-only
DIR="$(readlink -m "$(pwd)")"
echo $DIR
./configure --prefix=$DIR/..
cd ../

# build libevhtp
cd ./libevhtp/build
make
make install
cd ../../


# build ctemplate
cd ./ctemplate-read-only
make
make install
