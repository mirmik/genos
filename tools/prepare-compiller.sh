set -e
echo "prepare_compiler.sh"

sudo apt-get update
sudo apt install -y software-properties-common
sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo apt-get update

sudo apt install -yq g++-11 gcc-11
sudo rm -f /usr/bin/g++
sudo rm -f /usr/bin/gcc
sudo ln -s /usr/bin/g++-11 /usr/bin/g++
sudo ln -s /usr/bin/gcc-11 /usr/bin/gcc
