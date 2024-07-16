set -e

WORKDIR=/tmp
ARCHIEVE_SOURCE_NAME=artifacts
ARCHIEVE_SOURCE_DIR=$WORKDIR/$ARCHIEVE_SOURCE_NAME
ARCHIEVE_NAME=artifacts.tar.gz
ARCHIEVE_PATH=$WORKDIR/$ARCHIEVE_NAME
EDIR=$ARCHIEVE_SOURCE_DIR
RDIR=$ARCHIEVE_SOURCE_DIR/ROOT
CHECKSUM_FILE=$ARCHIEVE_SOURCE_DIR/CHECKSUM

function checksum_of_all_files {
    local directory=$1
    cd $directory
    local checksum_file=$CHECKSUM_FILE
    rm -f $checksum_file
    for file in `find . -type f`; do
        local sum=`md5sum $file`
        echo $sum >> $checksum_file
    done
    # remove emptylines from file
    sed -i '/^$/d' $checksum_file
    cd -
}

function create_signature_file {
    local signature_file=$1
    # get date as text 
    local date=`date`
    echo "FormatVersion: 1" > $signature_file
    echo "Date: $date" >> $signature_file
    local machinename=`uname -n`
    echo "Machine: $machinename" >> $signature_file
    local author=`git config user.name`
    echo "Author: $author" >> $signature_file
    local checksum=`md5sum $CHECKSUM_FILE`
    # total files
    local total=`cat $CHECKSUM_FILE | wc -l`
    echo "TotalFiles: $total" >> $signature_file
    checksum=`echo $checksum | awk '{print $1}'`
    echo "Checksum: $checksum" >> $signature_file
}

echo "RDIR: $RDIR"

rm -rf $ARCHIEVE_SOURCE_DIR
rm -f $ARCHIEVE_PATH
mkdir -p $RDIR/lib/x86_64-linux-gnu/
mkdir -p $RDIR/usr/lib/x86_64-linux-gnu/
mkdir -p $RDIR/usr/lib
mkdir -p $RDIR/usr/bin
mkdir -p $RDIR/usr/local/bin
mkdir -p $RDIR/etc/rfdaemon
mkdir -p $RDIR/etc/network
mkdir -p $RDIR/etc/systemd/system
mkdir -p $RDIR/home/rfmeas/web
mkdir -p $RDIR/home/rfmeas/project
mkdir -p $RDIR/etc/apt/apt.conf.d
touch $RDIR/home/rfmeas/project/__dirkeep__

# Add Applications
cp tools/bin/reproxy $RDIR/usr/bin/reproxy
cp tools/bin/uredir $RDIR/usr/bin/uredir
cp rfmeask/rfmeas $RDIR/usr/local/bin/rfmeas
cp tools/bin/rfdaemon $RDIR/usr/local/bin/rfdaemon
cp tools/bin/rfdaemonctl $RDIR/usr/local/bin/rfdaemonctl
#cp crow/ctrans $RDIR/usr/local/bin/ctrans
#cp crow/crowker $RDIR/usr/local/bin/crowker
cp dataproxy/dataproxy $RDIR/usr/local/bin/dataproxy
cp ModbusRemoteControl/ModbusRemoteControl $RDIR/usr/local/bin/ModbusRemoteControl
cp broadcast-door/shell.py $RDIR/usr/local/bin/bshell
#cp rfconfigurator/RFConfigurator $RDIR/usr/local/bin/rfconfigurator

# Add Libraries
cp /usr/lib/libigris.so $RDIR/usr/lib/libigris.so
cp /usr/lib/libnos.so $RDIR/usr/lib/libnos.so
cp /usr/lib/libcrow.so $RDIR/usr/lib/libcrow.so
cp /usr/lib/libralgo.so $RDIR/usr/lib/libralgo.so
cp /usr/lib/librabbit.so $RDIR/usr/lib/librabbit.so
cp /lib/x86_64-linux-gnu/libstdc++.so.6.* $RDIR/lib/x86_64-linux-gnu/
cp /usr/lib/x86_64-linux-gnu/libmodbus.so $RDIR/usr/lib/x86_64-linux-gnu/
cp /usr/lib/x86_64-linux-gnu/libmodbus.so* $RDIR/usr/lib/x86_64-linux-gnu/

# Add Configs
cp tools/positioner-configs/applications.json $RDIR/etc/rfdaemon/applications.json
cp tools/positioner-configs/rfdaemon.service $RDIR/etc/systemd/system/rfdaemon.service
cp tools/positioner-configs/20auto-upgrades $RDIR/etc/apt/apt.conf.d/20auto-upgrades
#cp tools/positioner-configs/interfaces $RDIR/etc/network/interfaces

# Add web-server assets
cp -r tools/positioner-assets/* $RDIR/home/rfmeas/web/

cp tools/environment.sh $EDIR/

checksum_of_all_files $RDIR/
create_signature_file $EDIR/SIGNATURE

tree $EDIR/

cd $WORKDIR
tar -czf $ARCHIEVE_NAME ./$ARCHIEVE_SOURCE_NAME
cd -
cp $ARCHIEVE_PATH ./artifacts.tar.gz

# echo SIGNATURE file 
cat $EDIR/SIGNATURE
