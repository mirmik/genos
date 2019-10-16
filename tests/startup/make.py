#!/usr/bin/env python3

import licant
import os

licant.include("genos")

licant.glbfunc.genos_firmware(
	sources=["start.cpp"],
	mdepends=[
		"igris.stdlibs",
		"genos.systime",
		("genos.hal.stm32.common", "startup_debug")],
	cxx_flags = "-Wall",
	cxxstd = "c++14"
)
