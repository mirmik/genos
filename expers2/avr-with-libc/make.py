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
		
		"genos.ktimer_manager",
		"genos.schedee_manager",
		"genos.systime",

		"igris.compat.std",
		"igris.compat.posix",

		"igris.syslock",
		"igris.flags.clean",

		"cpu.avr.atmega2560"
	],

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

licant.ex("firmware.hex")