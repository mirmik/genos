set -ex
list_of_directories="\
    igris \
    nos \
    crow \
    ralgo \
    rabbit \
    rfmeask \
    dataproxy \
    ModbusRemoteControl \
    zillot \
    genos"

sudo licant-libs ./igris
sudo licant-libs ./nos
sudo licant-libs ./crow
sudo licant-libs ./ralgo
sudo licant-libs ./rabbit
sudo licant-libs ./zillot
sudo licant-libs ./genos
sudo licant-libs ./onboardtest

(cd libmodbus; make; sudo cp src/.libs/libmodbus.a /usr/lib/)

(cd RxCpp; cmake .; cmake --build . --config Release; sudo cmake --build . --config Release --target install)
(cd cpp-httplib-static; cmake .; cmake --build . --config Release; sudo cmake --build . --config Release --target install)
(cd ircc; cmake .; cmake --build . --config Release; sudo cmake --build . --config Release --target install)

for directory in $list_of_directories
do
    echo "Making $directory"
    cd $directory
    if [ -f ./CMakeLists.txt ]; then
        cmake .
        cmake --build . --config Release

        sudo cmake --build . --config Release --target install || true
    else
        python3 ./make.py -jj
        if [ -f runtests ]; then
            ./runtests
        fi
        sudo python3 ./make.py install
    fi
        
    cd ..
done

cd helix-cnc
./make.py
cd ..

sudo cp ubuntu-20.04-x86_64-artifacts/reproxy /usr/bin/reproxy

