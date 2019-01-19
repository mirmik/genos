#!/usr/bin/env python3
#coding: utf-8

import licant
from licant.cxx_modules import application
from licant.modules import submodule
from licant.libs import include
from licant.cxx_make import make_gcc_binutils

import os

include("genos")
include("gxx")
binutils = make_gcc_binutils("avr")

application("firmware", 
	binutils = binutils,
	sources = ["main.c"],
	target = "firmware.bin",

	cxx_flags = "-Os -fpermissive -fno-threadsafe-statics -flto -pedantic",
	cc_flags = "-Os -flto  -Werror=all",

	include_modules = [
	#stdlibs
		submodule("gxx.posix"),
		submodule("gxx.libc"),
		submodule("gxx.std"),
		submodule("gxx.include"),
		submodule("gxx.c_only"),
		
	#gxx support
		submodule("gxx.dprint", "diag"),
		submodule("gxx.diag", "impl"),
		submodule("gxx.syslock", "genos.atomic"),

	#base and os support
		submodule("genos.include"),
		submodule("genos.errno"),
		submodule("genos.board", "crius_aiop2"),
		submodule("genos.irqtbl"),
		submodule("genos.systime"),
		submodule("genos.sched", "impl"),

		submodule("genos.drivers.spi.avr"),
		submodule("genos.drivers.gpio.avr"),

		submodule("genos.mvfs"),
		submodule("genos.mvfs.global"),
		submodule("genos.malloc", "lin"),
	]
)

@licant.routine
def install():
	os.system("avrdude -P/dev/ttyUSB0 -v -cwiring -patmega2560 -b115200 -D -Uflash:w:./firmware.bin -u")

licant.ex(default = "firmware")