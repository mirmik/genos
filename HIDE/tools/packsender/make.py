#!/usr/bin/env python
#coding: utf-8

from licant.cxx_modules import application, doit
from licant.libs import include
from licant.modules import submodule

include("gxx")

application("main", 
	sources = ["main.cpp"],
	include_modules = [
		submodule("gxx.include"),
		submodule("gxx"),
		submodule("gxx.dprint", "stdout"),
	]
)
doit("main")