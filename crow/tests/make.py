#!/usr/bin/env python3

import licant
from licant.cxx_modules import application
from licant.modules import submodule, module
from licant.libs import include

licant.include("crow", "../crow.g.py")

tests_c = [
]

application("runtests",
	sources = [
		"*.cpp",
		"brocker/*.cpp"
	],
	mdepends=["crow", "crow.udpgate"],

	cxxstd="gnu++17",
	ccstd="c11",
	cxx_flags = "-g -Werror=all -Werror=pedantic -Weffc++ -Wno-deprecated-declarations",
	cc_flags = "-g -Werror=all -Werror=pedantic -Wno-deprecated-declarations",

	include_paths = ["."],
	libs = ["igris", "nos", "pthread"],
)

licant.ex("runtests")
