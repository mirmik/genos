#!/usr/bin/env python3

import licant
import os

licant.include("nos")
licant.include("crow")
licant.include("genos")

licant.glbfunc.genos_firmware(
	sources=["main.cpp"],
	mdepends=[
		"genos.utility.contty", 
		"crow",
		("nos.current_ostream", "nullptr"),
		("crow.time", "__none__"),
		("crow.allocator", "pool")
	], 
	cxx_flags = "-Wall",
	cxxstd = "c++17"
)
