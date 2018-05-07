#!/usr/bin/env python
#coding: utf-8

import licant
from licant.cxx_modules import application
from licant.modules import submodule
from licant.libs import include
from licant.cxx_make import make_gcc_binutils
import licant.routine

import os

print(licant.util.green("Licant"))

include("genos")
include("gxx")
binutils = make_gcc_binutils("avr")

application("main", 
	binutils = binutils,
	sources = ["main.cpp"],
	target = "firmware.bin",

	cxx_flags = "-Os -fpermissive -fno-threadsafe-statics -flto",
	cc_flags = "-Os -flto",

	include_modules = [
		submodule("genos.include"),
		submodule("genos.board", "arduino_mega"),
		submodule("genos.irqtbl"),
		#submodule("genos.tasklet"),
		#submodule("genos.timer"),
		#submodule("genos.schedee"),
		submodule("genos.atomic", "irqs"),
		#submodule("genos.malloc", "lin"),
		
		submodule("gxx.libc"),
		submodule("gxx.std"),
		submodule("gxx.include"),
		
		submodule("gxx.dprint", "diag"),
		submodule("gxx.diag", "impl"),
		submodule("gxx.panic", "abort"),
		#submodule("gxx.format"),
	]
)

licant.routine.add_routine("install", lambda x: os.system("./install.sh"))

licant.ex(default = "main")