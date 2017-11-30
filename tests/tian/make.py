#!/usr/bin/env python
#coding: utf-8

from licant.cxx_modules import application, doit
from licant.modules import submodule
from licant.libs import include
from licant.cxx_make import make_gcc_binutils
import licant.routine

import os

print(licant.util.green("Licant"))

include("genos")
include("gxx")
binutils = make_gcc_binutils("arm-none-eabi")

application("main", 
	binutils = binutils,
	sources = ["main.cpp"],
	target = "firmware.elf",

#	cxx_flags = "-Os -fpermissive -fno-threadsafe-statics -flto",
#	cc_flags = "-Os -flto",

	include_modules = [
		submodule("genos.hal.chip", "samd21g18"),
		submodule("genos.include"),
#		submodule("genos.board", "arduino_mega"),
#		submodule("genos.irqtbl"),
#		submodule("genos.tasklet"),
#		submodule("genos.timer"),
#		submodule("genos.schedee"),
#		submodule("genos.atomic", "irqs"),
#		submodule("genos.malloc", "lin"),
#		
		submodule("gxx.libc"),
#		submodule("gxx.std"),
		submodule("gxx.include"),
#		
#		submodule("gxx.dprint", "diag"),
#		submodule("gxx.diag", "impl"),
#		submodule("gxx.panic", "abort"),
		#submodule("gxx.format"),

		submodule("gxx.cxx_support"),
	]
)

#licant.routine.add_routine("install", lambda x: os.system("./install.sh"))

doit("main")