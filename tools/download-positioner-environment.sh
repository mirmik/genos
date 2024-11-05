set -e

sudo python3 -m pip install licant --upgrade
python3 -m pip install licant --upgrade

function update_or_clone() 
{
    if [ -d $1 ]; then
        echo "update $1"
        cd $1
        git pull 
        git submodule update --init 
        cd ..
    else
        echo "clone $1"
        git clone ${2} ${1} --recurse-submodules 
    fi
}

function update_or_clone_with_branch() 
{
    if [ -d $1 ]; then
        echo "update $1"
        cd $1
        git pull 
        git submodule update --init 
        cd ..
    else
        echo "clone $1"
        git clone ${2} ${1} --recurse-submodules --branch ${3} 
    fi
}

sudo python3 -m pip install licant --upgrade
python3 -m pip install licant --upgrade --user

# Неактуально после перехода на монорепу.
# update_or_clone netricks-20.04-env https://github.com/mirmik/netricks-20.04-env-docker
# update_or_clone igris https://github.com/smitek-robotics/igris
# update_or_clone nos https://github.com/smitek-robotics/nos
# update_or_clone crow https://github.com/smitek-robotics/crow
# update_or_clone ralgo https://github.com/smitek-robotics/ralgo
# update_or_clone rabbit https://github.com/smitek-robotics/rabbit
# update_or_clone RxCpp https://github.com/smitek-robotics/RxCpp
# update_or_clone cpp-httplib-static https://github.com/mirmik/cpp-httplib-static
# update_or_clone rfmeask https://github.com/smitek-robotics/rfmeask
# update_or_clone rfmeask_drv https://github.com/smitek-robotics/rfmeask_drv
# update_or_clone rfdaemon https://github.com/smitek-robotics/rfdaemon
# update_or_clone dataproxy https://github.com/smitek-robotics/rfmeask_dataproxy
# update_or_clone ModbusRemoteControl https://github.com/smitek-robotics/ModbusRemoteControl
# update_or_clone ubuntu-20.04-x86_64-artifacts https://github.com/smitek-robotics/ubuntu-20.04-x86_64-artifacts
# update_or_clone ircc https://github.com/mirmik/ircc
# update_or_clone onboardtest https://github.com/mirmik/onboardtest
# update_or_clone zillot https://github.com/smitek-robotics/cpulib
# update_or_clone genos https://github.com/smitek-robotics/genos
# update_or_clone helix-cnc https://github.com/smitek-robotics/helix
# update_or_clone libmodbus https://github.com/stephane/libmodbus
# update_or_clone broadcast-door https://github.com/mirmik/broadcast-door

cd libmodbus
./autogen.sh
./configure --disable-shared
cd ..

