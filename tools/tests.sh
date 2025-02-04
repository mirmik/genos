set -ex
list_of_directories="\
    igris \
    nos \
    crow \
    ralgo \
    genos \
    rabbit \
    rfmeask \
    rfdaemon \
    dataproxy \
    ModbusRemoteControl \
    zillot \
    genos"

for directory in $list_of_directories
do
    echo "Test $directory"
    cd $directory
    
    #concat $directory and _test
    test_name=$directory"_test"

    if [ -f ./runtests ]; then
        echo "Run runtests"
        ./runtests
    elif [ -f ./$test_name ]; then
        echo "Run $test_name"
        ./$test_name
    fi
        
    cd ..
done


