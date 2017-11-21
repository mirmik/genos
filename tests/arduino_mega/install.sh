sudo avrdude -C/home/mirmik/soft/arduino-1.8.5/hardware/tools/avr/etc/avrdude.conf -P/dev/ttyUSB0 -cwiring -patmega2560 -b57600 -D -Uflash:w:./firmware.bin -v -v

#sudo /home/mirmik/soft/arduino-1.8.5/hardware/tools/avr/bin/avrdude -C/home/mirmik/soft/arduino-1.8.5/hardware/tools/avr/etc/avrdude.conf -P/dev/ttyUSB0 -cwiring -patmega2560 -b57600 -D -Uflash:w:./firmware.bin -v -v

#/home/mirmik/soft/arduino-1.8.5/hardware/tools/avr/bin/avrdude -C/home/mirmik/soft/arduino-1.8.5/hardware/tools/avr/etc/avrdude.conf -v -v -patmega2560 -cwiring -P/dev/ttyUSB0 -b57600 -Uflash:w:/tmp/arduino_build_70066/Blink.ino.hex:i 
