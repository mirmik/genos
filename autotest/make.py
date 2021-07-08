#!/usr/bin/env python3

import sys
import licant
from licant.cxx_modules import application
from licant.modules import submodule, module
from licant.libs import include

licant.execute("../genos.g.py")

application("runtests",
	sources = ["*.cpp"],

	cxx_flags = "-g",
	cc_flags = "-g",

	include_paths = ["."],
	mdepends=[
		"genos.include",
		"genos.sched2",
		"genos.systime"
	],
	libs = ["nos", "igris"]
)

licant.ex("runtests")
