# fetch git submodules
git submodule init
git submodule update

# zlib (optional)
# cd deps/zlib
# ./configure
# make
# rm *.dylib
# cd ../..

# wolfssl
cd deps/wolfssl
./autogen.sh
./configure --prefix=${PWD} --enable-opensslextra --enable-static
make
make install
cd ../..

# libwebsockets
cd deps/libwebsockets
mkdir build
cd build 
cmake .. -DCMAKE_INSTALL_PREFIX:PATH=./ -DLWS_USE_WOLFSSL=1 -DLWS_WOLFSSL_INCLUDE_DIRS=../../wolfssl -DLWS_WOLFSSL_LIBRARIES=../../wolfssl/lib/libwolfssl.a ..
make
mkdir ../install
cp lib/libwebsockets.a ../
mkdir ../include
cp ../lib/libwebsockets.h ../include
cp ./lws_config.h ../include
cd ..
cd ../..

# msgpack
cd deps/msgpack-c
cmake .
make
rm *.dylib
cd ../..