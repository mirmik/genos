#!/usr/bin/env python3

import licant

licant.include("genos")
licant.include("igris")
licant.include("nos")

licant.cxx_application("firmware",
	binutils = licant.cxx_make.make_gcc_binutils("arm-none-eabi"),
	sources = ["main.c"], 

	mdepends = [
		"genos.include",

		"igris.include",
		"igris.libc",

		"igris.std",
		"igris.posix",
		"igris.bug",
		
		("igris.syslock", "genos.atomic"),
		("igris.dprint", "diag"),

		("genos.board", "nucleo-f401re"),
		"genos.drivers.stm32",

#		"nos.include",
#		"nos.print",
#		("nos.current_ostream", "nullptr"),

		("genos.sched"),
		("genos.malloc", "lin"),

		#"igris.cxx_support",

		"genos.utility.contty",
		"genos.utility.mshell"
	],

	cxx_flags = "-O3",
)

licant.ex("firmware")