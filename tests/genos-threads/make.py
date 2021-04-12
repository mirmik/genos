#!/usr/bin/env python3

import licant
import os

licant.include("genos")

licant.glbfunc.genos_firmware(
	sources=["main.cpp"],
	mdepends=["igris.stdlibs"],
	cxx_flags = "-Wall",
	cxxstd = "c++14",
	optimize="-O2",

	defines = ["SCHED_DEBUG=1"]
)
