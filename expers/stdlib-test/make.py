#!/usr/bin/env python3

import licant
import os

licant.include("genos")

licant.glbfunc.genos_firmware(
	sources=["main.cpp"],
	mdepends = [
		("genos.hal.stm32.common", "experiment"),
			"newlib-stub", 
		
		#"igris.libc", "igris.std", "igris.posix"
	]
)
