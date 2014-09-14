cd loopy-redis
git checkout master
cd ..
cd ./third_party/hiredis
make
cd ..
mkdir -p lib
cp hiredis/libhiredis* ./lib
ln -s libhiredis.so ./lib/libhiredis.so.0.11
