#!/usr/bin/env python3

import sys
import licant
from licant.cxx_modules import application
from licant.modules import submodule, module
from licant.libs import include

licant.execute("../genos.g.py")

application("runtests",
	sources = [
		"main.cpp",
		"drivers_gpio_devices.cpp",
	],
	cxx_flags = "-g",
	cc_flags = "-g",
	include_paths = ["."],
	mdepends = [ "igris", "genos.include" ],
)

application("atmega_firmware",
	toolchain=licant.cxx_make.toolchain_gcc("avr-"),
	builddir="build_avr",
	sources=[
		"atmega_main.cpp"
	],
	mdepends=[
		"genos.include",

		"board.arduino_mega",
		"cpu.avr.atmega2560",

		("igris.dprint", "diag"),
		"igris.bug",
		"igris.cxx_support",

		"drivers.avr.gpio",
	]
)

application("atmega_firmware",
	toolchain=licant.cxx_make.toolchain_gcc("avr-"),
	builddir="build_avr",
	sources=[
		"atmega_main.cpp"
	],
	mdepends=[
		"genos.include",

		"board.arduino_mega",
		"cpu.avr.atmega2560",

		("igris.dprint", "diag"),
		"igris.bug",
		"igris.cxx_support",

		"drivers.avr.gpio",
	]
)

application("stm32f401_firmware",
	toolchain=licant.cxx_make.toolchain_gcc("arm-none-eabi-"),
	builddir="build_stm32f401",
	sources=[
		"stm32f401_main.cpp"
	],
	mdepends=[
		"genos.include",

		"board.nucleo-f401re",
		"cpu.stm32.stm32f401re",

		("igris.dprint", "diag"),
		"igris.bug",
		"igris.cxx_support",
		("igris.syslock", "genos.atomic"),
		#"igris.compat.libc",

		#"drivers.avr.gpio",
		"genos.irqtable",
		"genos.systime",
		"genos.newlib_bind",
		"genos.sched",
	]
)


licant.fileset("all", [
	"runtests", 
	"atmega_firmware",
	"stm32f401_firmware"
])

licant.ex("all")

