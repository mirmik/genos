mkdir build-android
cd build-android
cmake .. -B . \
	-DCMAKE_CXX_COMPILER=${ACOMPILERS}/aarch64-linux-android30-clang++ \
	-DCMAKE_C_COMPILER=${ACOMPILERS}/aarch64-linux-android30-clang \
	-DWITHOUT_TESTS=1 -DSTATIC_LIBS=1
make