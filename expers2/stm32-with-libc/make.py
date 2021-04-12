#!/usr/bin/env python3

import licant
import os
from licant.cxx_make import toolchain_gcc

TOOLCHAIN = toolchain_gcc("arm-none-eabi-")

licant.include("genos")
licant.include("igris")

licant.cxx_application("firmware.elf",
	toolchain=TOOLCHAIN,
	sources = ["main.cpp"],
	
	mdepends = [
		"genos.include",
		"genos.compat.newlib-syscalls.stm32",

		"genos.ktimer_manager",
		"genos.schedee_manager",
		"genos.systime",

		#"igris.compat.std",
		#"igris.compat.posix",

		"igris.syslock",
		#"igris.flags.clean",
		"igris.dprint",
		"igris.bug",
		"igris.util",

		"cpu.stm32.stm32f401re"
	],

	cxx_flags="",
	cc_flags="",
	ld_flags=""
)

licant.objcopy(
	toolchain=TOOLCHAIN,
	src="firmware.elf",
	tgt="firmware.hex", 
	format="ihex",
	sections=[".text"])

licant.ex("firmware.hex")