#!/usr/bin/env python3

import licant
import os

licant.include("genos")
licant.include("igris")

#gccprefix = "arm-none-eabi"
#boardname = "nucleo-f401re"
#gpiodrivers = "genos.drivers.stm32_gpio"

#gccprefix = "avr"
#boardname = "crius_aiop2"
#gpiodrivers = "genos.drivers.avr.gpio"

gccprefix = "avr"
boardname = "arduino_uno"
gpiodrivers = "genos.drivers.avr.gpio"

licant.cxx_application("firmware",
	binutils = licant.cxx_make.make_gcc_binutils(gccprefix),
	sources = ["main.c"], 

	mdepends = [
		"genos",
		"genos.sched",
		("genos.malloc", "lin"),

		("genos.board", boardname),
		gpiodrivers
	],

	cxx_flags = "-O3",
)

@licant.routine
def install():
	#os.system("arm-none-eabi-objcopy -O binary firmware firmware.bin")
	#os.system("sudo st-flash write firmware.bin 0x8000000")
	#os.system("avrdude -P/dev/ttyACM0 -v -carduino -patmega2560 -b115200 -D -Uflash:w:./firmware -u")
	os.system("avrdude -P/dev/ttyACM0 -v -carduino -patmega328p -b115200 -D -Uflash:w:./firmware -u")

@licant.routine
def terminal():
	os.system("sudo gtkterm -p /dev/ttyACM0 -s 115200 --parity none")

licant.ex("firmware")