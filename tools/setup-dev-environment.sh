sudo apt update
sudo apt upgrade -y
sudo apt install -y \
	g++ \
	net-tools \
	cmake \
	ifupdown \
	stlink-tools \
	tuptime bridge-utils \
	ssh \
	libreadline-dev \
	libmodbus-dev \
	apt-utils \
        qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools libqt5charts5-dev \
        libjsoncpp-dev \
        libfmt-dev \
        libopengl-dev \
        libglew-dev \
        libglfw3-dev \
        libglm-dev \
        libassimp-dev \
        nano \
        tree \
	gcc-arm-none-eabi \
	avr-libc \
	gcc-avr \
	gdb-multiarch \
	avrdude \
	autoconf \
	libtool \
	python3-pip

sudo systemctl disable network-manager.service
sudo systemctl disable networkd-dispatcher.service
sudo systemctl enable networking.service
sudo python3 -m pip install gitpython
