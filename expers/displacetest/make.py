#!/usr/bin/env python3

import licant
import os

licant.include("genos")

licant.glbfunc.genos_firmware(
	sources=["main.c"],
	mdepends=[
		"genos.sched",
		("genos.wait", "static"),
		"genos.utility.contty2",
		"genos.utility.mshell"
	] 
)
