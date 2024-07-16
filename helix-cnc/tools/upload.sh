set -xe

DIR_MULTIAX=$HELIX_BOARD_FIRMWARE_PATH_MULTIAX
DIR_TWOAXIS=$HELIX_BOARD_FIRMWARE_PATH_TWOAXIS
date=`date +%Y.%m.%d.%H.%M.%S`

arm-none-eabi-objcopy -O binary firmware-multiax.elf firmware-multiax.bin
cp firmware-multiax.bin "$DIR_MULTIAX/firmware-multiax-$date.bin"
cp firmware-multiax.bin "$DIR_MULTIAX/firmware-multiax-latest.bin"

arm-none-eabi-objcopy -O binary firmware-mini.elf firmware-mini.bin
cp firmware-mini.bin "$DIR_TWOAXIS/firmware-mini-$date.bin"
cp firmware-mini.bin "$DIR_TWOAXIS/firmware-mini-latest.bin"