#!/usr/bin/env python3

import licant

licant.include("genos")
licant.include("igris")
licant.include("nos")

arm_binutils = licant.cxx_make.make_gcc_binutils("arm-none-eabi")

licant.cxx_application("firmware.bin",
	binutils = arm_binutils,
	sources = ["src/main.cpp"], 

	mdepends = [
		"genos.include",
		"genos.irqtbl",
	],

	cxx_flags = "-O3",
)

licant.ex("firmware.bin")