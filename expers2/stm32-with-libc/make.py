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
		"genos.irqtable",	

		"igris.compat.std",
		"igris.compat.posix",
		"igris.compat.libc",

		"igris.syslock",
		"igris.flags.clean",
		"igris.dprint",
		"igris.bug",
		"igris.util",

		"cpu.stm32.stm32g474re",
		"board.nucleo-g474.init",
	],

	cc_flags = "-g -Wl,--gc-sections -nostdlib -nostdinc -fdata-sections -ffunction-sections -Wl,--gc-sections -flto",
	cxx_flags = "-g -Wl,--gc-sections -nostdlib -nostdinc -fdata-sections -ffunction-sections -Wl,--gc-sections -flto -fno-rtti -fno-exceptions -fno-threadsafe-statics   -ffunction-sections -fno-rtti -flto -fno-use-cxa-atexit",
	ld_flags = "-g -nostdlib -fno-rtti -fno-exceptions -fdata-sections -ffunction-sections -flto -Wl,--gc-sections -fno-use-cxa-atexit",
	optimize = "-Os",
)

licant.objcopy(
	toolchain=TOOLCHAIN,
	src="firmware.elf",
	tgt="firmware.bin", 
	format="binary",
	sections=[".text", ".data"])

@licant.routine(deps=["firmware.bin"])
def install():
	#os.system("st-flash erase")
	os.system("st-flash --reset write firmware.bin 0x8000000")
	#os.system("st-flash reset")

@licant.routine
def terminal(path="/dev/ttyACM0"):
	os.system("gtkterm -p {} -s 115200 --parity none".format(path))

licant.ex("firmware.bin")