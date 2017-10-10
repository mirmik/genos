#!/usr/bin/env python
#coding: utf-8

from licant.cxx_modules import application, doit
from licant.modules import submodule
from licant.libs import include
from licant.cxx_make import make_gcc_binutils
import licant.routine

import os

include("genos")
include("gxx")
binutils = make_gcc_binutils("avr")

application("main", 
	binutils = binutils,
	sources = ["main.cpp"],
	target = "firmware.bin",

	cxx_flags = "-Og",
	cc_flags = "-Og",

	include_modules = [
		submodule("genos.include"),
		submodule("genos.board", "arduino_mega"),
		submodule("genos.irqtbl"),
		
		submodule("gxx.libc"),
		submodule("gxx.include"),
		submodule("gxx.dprint", "stub"),
		submodule("gxx.diag", "stub"),
	]
)

licant.routine.add_routine("install", lambda x: os.system("./install.sh"))

doit("main")