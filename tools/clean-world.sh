set -e
list_of_directories=$(cat tools/list_of_directories.txt)

cd RxCpp
git clean -xfd
cd ..

cd cpp-httplib-static
git clean -xfd
cd ..

for directory in $list_of_directories
do
    echo "Cleaning $directory"
    cd $directory
    git clean -xfd
    cd ..
done
