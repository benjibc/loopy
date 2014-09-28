# install all the necessary libraries for folly
sudo apt-get install \
    g++ \
    automake \
    autoconf \
    autoconf-archive \
    libtool \
    libboost-all-dev \
    libevent-dev \
    libdouble-conversion-dev \
    libgoogle-glog-dev \
    libgflags-dev \
    liblz4-dev \
    liblzma-dev \
    libsnappy-dev \
    make \
    zlib1g-dev \
    binutils-dev \
    libiberty-dev \
    libjemalloc-dev

#!/bin/bash
DIR="$(dirname $0)"

cd ./third_party
git submodule update --init --recursive

# configure libevhtp
cd ./libevhtp
git pull origin master 
git checkout master
cd ./build
cmake .. -DCMAKE_INSTALL_PREFIX:PATH=$DIR/../../third_party -DEVHTP_DISABLE_REGEX=ON
cd ../../

# configure ctemplate
cd ./ctemplate
git pull origin master 
git checkout master
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
cd ./ctemplate
make
make install
cd ..

# build folly
cd ./folly/folly
DIR="$(readlink -m "$(pwd)")"
autoreconf -ivf
./configure --prefix=$DIR/../..
make
make check
make install
cd ../../
