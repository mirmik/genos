set -xe

./runtests
./rfmeas --simtest 0
./rfmeas --simtest 1
