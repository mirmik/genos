#!/usr/bin/env python3

import licant
import licant.cxx_make
import os

licant.include("genos")
licant.include("igris")
licant.include("nos")

binutils = licant.cxx_make.make_gcc_binutils("avr")

licant.cxx_application("firmware", 
	binutils = binutils,
	sources = ["main.cpp"],
	target = "firmware.bin",

	cxx_flags = "-Os -fpermissive -fno-threadsafe-statics -flto",
	cc_flags = "-Os -flto",

	mdepends = [
		"genos.include",
		"genos.irqtbl",
		"genos.systime",
		("genos.board", "crius_aiop2"),

		"igris.include",
		"igris.libc",
		"igris.posix",
		"igris.std",
		("igris.syslock", "genos.atomic"),
		("igris.dprint", "diag"),
		
		"genos.drivers.common",
		"genos.drivers.gpio.avr", 
		"genos.drivers.usart.avr",

		"nos",
		("nos.current_ostream", "nullptr"),
	]
)

@licant.routine
def install():
	os.execute("sudo avrdude -P/dev/ttyUSB0 -v -cwiring -patmega2560 -b115200 -D -Uflash:w:./firmware.bin -u")

licant.ex(default = "firmware")