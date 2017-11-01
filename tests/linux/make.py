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

application("main", 
	sources = ["main.cpp"],

	cxx_flags = "-Os -fpermissive -fno-threadsafe-statics -flto",
	cc_flags = "-Os -flto",

	include_modules = [
		submodule("genos.include"),
		submodule("genos.board", "native"),
		submodule("genos.tasklet"),
		submodule("genos.timer"),
		submodule("genos.schedee"),
		submodule("genos.atomic", "mutex"),
		
		submodule("gxx.include"),
		
		submodule("gxx.dprint", "stdout"),
		submodule("gxx.panic", "abort"),
		submodule("gxx.format"),
	]
)

doit("main")