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
binutils = make_gcc_binutils("avr")

application("main", 
	binutils = binutils,
	sources = ["main.cpp"],
	target = "firmware.bin",

	cxx_flags = "-Os -fpermissive -fno-threadsafe-statics -flto",
	cc_flags = "-Os -flto",

	include_modules = [
		submodule("gxx.diag", "impl"),
	]
)

licant.routine.add_routine("install", lambda x: os.system("./install.sh"))

doit("main")