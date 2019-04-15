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
		#"genos.irqtbl",

		"igris.include",
		"igris.libc",
		"igris.std",
		"igris.posix",
		"igris.bug",
		("igris.dprint", "diag"),

		("genos.board", "rl_syncro_v2"),
		"genos.drivers.stm32"
	],

	cxx_flags = "-O3",
)

licant.ex("firmware")