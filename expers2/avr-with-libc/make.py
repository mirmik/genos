#!/usr/bin/env python3

import licant
import os
from licant.cxx_make import toolchain_gcc

TOOLCHAIN = toolchain_gcc("avr-")

licant.include("genos")
licant.include("igris")

licant.cxx_application("firmware.elf",
	toolchain=TOOLCHAIN,
	sources = ["main.cpp"],
	
	mdepends = [
		"genos.include",

		"igris.compat.std",
		"igris.compat.posix",
		"igris.util",
		"igris.dprint",
		"igris.cxx_support",
		"igris.flags.clean",

		"cpu.avr.atmega2560",
		"board.arduino_mega",

		"genos.drivers.avr"
	],

	defines = ["F_CPU=16000000"],

	cxx_flags="-mmcu=atmega2560",
	cc_flags="-mmcu=atmega2560",
	ld_flags="-mmcu=atmega2560"
)

licant.objcopy(
	toolchain=TOOLCHAIN,
	src="firmware.elf",
	tgt="firmware.hex", 
	format="ihex",
	sections=[".text"])

@licant.routine(deps=["firmware.hex"])
def install():
	os.system("avrdude avrdude -P/dev/ttyACM0 -v -cwiring -patmega2560 -b115200 -D -Uflash:w:./firmware.hex -u")

@licant.routine
def terminal(path="/dev/ttyACM0"):
	os.system("sudo gtkterm -p {} -s 115200 --parity none".format(path))

licant.ex("firmware.hex")