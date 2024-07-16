set -xe

DIR=$SCANNING_BOARD_FIRMWARE_PATH
date=`date +%Y.%m.%d.%H.%M.%S`

arm-none-eabi-objcopy -O binary firmware.elf firmware.bin
cp firmware.bin "$DIR/firmware-$date.bin"
cp firmware.bin "$DIR/firmware-latest.bin"