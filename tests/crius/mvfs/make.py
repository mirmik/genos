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

	cxx_flags = "-Os -fpermissive -fno-threadsafe-statics -flto",
	cc_flags = "-Os -flto",

	include_modules = [
		submodule("genos.include"),
		submodule("genos.board", "arduino_mega"),
		submodule("genos.irqtbl"),
		submodule("genos.systime"),
		submodule("genos.sched", "impl"),

		submodule("gxx.syslock", "genos.atomic"),
		
		submodule("gxx.posix"),
		submodule("gxx.libc"),
		submodule("gxx.std"),
		submodule("gxx.include"),
		
		submodule("gxx.dprint", "diag"),
		submodule("gxx.diag", "impl"),
#		submodule("gxx.c_only"),
	]
)

@licant.routine
def install():
	os.system("sudo avrdude -P/dev/ttyUSB0 -v -cwiring -patmega2560 -b115200 -D -Uflash:w:./firmware.bin -u")

licant.ex(default = "firmware")