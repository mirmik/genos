#!/usr/bin/env python3
#coding: utf-8

import licant
import licant.install
from licant.cxx_modules import application
from licant.libs import include
import os

licant.include("ralgo", "../../ralgo.g.py")

defines = ["NOTRACE=1", "HEIMER_CROW_SUPPORT=1"]

application("sigtrans", 
	builddir = "build" if __name__ == "__main__" else "build/sigtrans",
	sources = [
		"main.cpp"
	],
	mdepends= [
		"ralgo",
	],
	defines = defines,
	cxx_flags = "-g",
	cc_flags = "-g",
	libs = ["pthread", "readline", "igris", "nos", "crow"],
	cxxstd = "c++17",
)


#@licant.routine
#def doc():
#	os.system("cd doc; ./make.sh")

licant.install.install_application(tgt="install_sigtrans", src="sigtrans", dst="sigtrans")
	
if __name__ == "__main__":
	licant.install.install_application(tgt="install", src="sigtrans", dst="sigtrans")
	licant.ex("sigtrans")
