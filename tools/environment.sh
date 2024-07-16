set -ex

sudo apt update
sudo apt upgrade -y
sudo apt install -y \
	net-tools \
	ifupdown \
	stlink-tools \
	tuptime \
	bridge-utils \
	apt-utils \
	nano \
    tree \
	rlwrap \
	mosquitto \
	mosquitto-clients \
	gdb-multiarch

sudo python3 -m pip install gitpython
