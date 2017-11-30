arm-none-eabi-objcopy -O binary  firmware.elf firmware.bin 
arm-none-eabi-objcopy -O ihex -R .eeprom  firmware.elf firmware.hex
avrdude -v -v -patmega2560 -cwiring -P/dev/ttyUSB0 -b57600 -Uflash:w:firmware.hex:i -u