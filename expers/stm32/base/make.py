#!/usr/bin/env python3

import licant

licant.include("genos")
licant.include("igris")
licant.include("nos")

licant.cxx_application("firmware",
	binutils = licant.cxx_make.make_gcc_binutils("arm-none-eabi"),
	sources = ["main.cpp"], 

	mdepends = [
		"genos.include",

		"igris.include",
		"igris.libc",
		"igris.std",
		"igris.posix",
		"igris.bug",
		("igris.syslock", "genos.atomic"),
		("igris.dprint", "diag"),

		("genos.board", "stm32f4discovery"),
		"genos.drivers.stm32"
	],

	cxx_flags = "-O3",
)

licant.ex("firmware")