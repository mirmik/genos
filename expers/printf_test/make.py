#!/usr/bin/env python3
#coding: utf-8

import licant
import licant.libs
import licant.cxx_make
from licant.cxx_modules import application
from licant.modules import submodule

import os

licant.libs.include("genos")
licant.libs.include("gxx")
binutils = licant.cxx_make.make_gcc_binutils("avr")

application("firmware", 
	binutils = binutils,
	sources = ["main.cpp"],
	target = "firmware.bin",

	cxx_flags = "-Os -fpermissive -fno-threadsafe-statics -flto",
	cc_flags = "-Os -flto",

	include_modules = [
		submodule("genos"),
		submodule("genos.include"),
		submodule("genos.board", "arduino_mega"),
		submodule("genos.irqtbl"),
		
		submodule("gxx.posix"),
		submodule("gxx.libc"),
		submodule("gxx.std"),
		submodule("gxx.include"),

		submodule("gxx.util"),
		
		submodule("gxx.dprint", "diag"),
		submodule("gxx.diag", "impl"),
		submodule("gxx.panic", "abort"),
	]
)

@licant.routine
def install():
	#os.system("sudo avrdude -P/dev/ttyUSB0 -v -cwiring -patmega2560 -b115200 -D -Uflash:w:./firmware.bin -u")
	os.system("avrdude -P/dev/ttyUSB0 -v -cwiring -patmega2560 -b115200 -D -Uflash:w:./firmware.bin -u")

@licant.routine
def terminal():
	os.system("gtkterm -p /dev/ttyUSB0 -s 115200")

licant.ex(default = "firmware")