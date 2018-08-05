#!/usr/bin/python
#coding: utf-8

import licant
from licant.cxx_modules import application
from licant.libs import include
from licant.cxx_make import make_gcc_binutils

import os

include("genos")
include("gxx")
include("g1")
include("g0")
binutils = make_gcc_binutils("avr")

application("main", 
	binutils = binutils,
	sources = ["main.cpp"],
	target = "firmware.bin",

	cxx_flags = "-Os -fpermissive -fno-threadsafe-statics -flto",
	cc_flags = "-Os -flto",

	include_modules = [
		("genos"),
		("genos.board", "crius"),

		
		("genos.irqtbl"),
		("genos.tasklet"),
		("genos.timer"),
		("genos.schedee"),
		("gxx.syslock", "genos.atomic"),
		("genos.malloc", "lin"),
		
		("gxx.libc"),
		("gxx.std"),
		("gxx.include"),
		("gxx.util"),
		("gxx.log2", "stub"),
		
		("gxx.diag", "impl"),
		("gxx.dprint", "diag"),
		("gxx.print", "dprint"),

		("gxx.panic", "abort"),

		("g1"),
		("g1.allocator", "malloc"),

		("g0"),
	]
)

@licant.routine
def install():
	os.system("sudo avrdude -P/dev/ttyUSB0 -v -cwiring -patmega2560 -b115200 -D -Uflash:w:./firmware.bin -u")
	#os.system("sudo avrdude -P/dev/ttyACM0 -v -cwiring -patmega2560 -b115200 -D -Uflash:w:./firmware.bin -u")

@licant.routine
def terminal():
	os.system("sudo gtkterm -p /dev/ttyACM0 -s 115200")

licant.ex(default = "main", colorwrap = True)