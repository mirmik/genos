#!/usr/bin/python3
#coding: utf-8

import licant
from licant.cxx_modules import application
from licant.libs import include
from licant.cxx_make import make_gcc_binutils
from licant.modules import submodule

import time
import os

include("genos")
include("igris")
binutils = make_gcc_binutils("avr")

application("main", 
	binutils = binutils,
	sources = ["main.cpp"],
	target = "firmware.bin",

	cxx_flags = "-Os -fpermissive -fno-threadsafe-statics -flto -DNDEBUG",
	cc_flags = "-Os -flto -DNDEBUG",

	mdepends = 
	[
		("genos.board", "arduino_mega"),
		"genos",

		"genos.drivers.avr"
	]
)

@licant.routine(deps=["main"])
def install():
	os.system("avrdude -P/dev/ttyACM0 -v -cwiring -patmega2560 -b115200 -D -Uflash:w:./firmware.bin -u")

@licant.routine
def terminal():
	os.system("sudo gtkterm -p /dev/ttyACM0 -s 115200")

licant.ex("main")
