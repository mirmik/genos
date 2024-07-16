ARG ARCH
FROM $ARCH/ubuntu

RUN touch update_flag_1
RUN export DEBIAN_FRONTEND="noninteractive"

RUN apt-get update &&  \
            DEBIAN_FRONTEND=noninteractive apt-get -yq install keyboard-configuration
RUN apt-get update && \
      apt-get -qq -y install apt-utils sudo software-properties-common \
      qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools \
      libreadline-dev \
      libjsoncpp-dev \
      libfmt-dev \
      libmodbus-dev \
      libopengl-dev \
      libglew-dev \
      libglfw3-dev \
      libglm-dev \
      libassimp-dev \
      nano \
      tree

RUN add-apt-repository -y ppa:ubuntu-toolchain-r/test

RUN apt update -y
RUN apt -qq install git cmake python3 python3-pip -y
RUN sudo python3 -m pip install licant
RUN sudo python3 -m pip install gitpython

RUN sudo apt -qq install -y g++-11 gcc-11
RUN sudo rm -f /usr/bin/g++
RUN sudo rm -f /usr/bin/gcc
RUN sudo ln -s /usr/bin/g++-11 /usr/bin/g++
RUN sudo ln -s /usr/bin/gcc-11 /usr/bin/gcc

WORKDIR /root 
RUN git clone https://github.com/ReactiveX/RxCpp
RUN git clone https://github.com/yhirose/cpp-httplib
RUN git clone https://github.com/mirmik/cpp-httplib-static
RUN git clone https://github.com/mirmik/ircc

WORKDIR /root/RxCpp
RUN cmake .
RUN cmake --build . --config Release
RUN sudo cmake --build . --config Release --target install

WORKDIR /root/cpp-httplib
RUN cmake .
RUN cmake --build . --config Release
RUN sudo cmake --build . --config Release --target install

WORKDIR /root/cpp-httplib-static
RUN cmake .
RUN cmake --build . --config Release
RUN sudo cmake --build . --config Release --target install

WORKDIR /root/ircc
RUN cmake .
RUN cmake --build . --config Release
RUN sudo cmake --build . --config Release --target install

ADD ./sanitize-check.sh /root/sanitize-check.sh