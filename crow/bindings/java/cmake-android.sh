mkdir build-android
cd build-android
cmake .. -B . \
	-DCMAKE_CXX_COMPILER=${ACOMPILERS}/aarch64-linux-android29-clang++ \
	-DCMAKE_C_COMPILER=${ACOMPILERS}/aarch64-linux-android29-clang \
	-DIGRIS_LIBRARY_PATH=${IGRIS_LIBRARY} \
	-DNOS_LIBRARY_PATH=${NOS_LIBRARY} \
	-DCROW_LIBRARY_PATH=${CROW_LIBRARY} \
	-DSTATIC_LIBS=1
make --debug