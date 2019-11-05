#!/usr/bin/env python3

import licant
import os

licant.include("genos")

licant.glbfunc.genos_firmware(
	sources=["main.cpp", "mcp23s08.cpp"],
	mdepends=["arduino-compat", "igris.stdlibs", "igris.dprint"],
	cxx_flags = "-Wall",
	cxxstd = "c++14"
)
