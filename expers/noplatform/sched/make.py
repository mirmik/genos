#!/usr/bin/env python3

import licant

licant.include("genos")
licant.include("igris")

licant.cxx_application("target",
	sources = ["main.cpp"],
	mdepends= [
		("genos.sched", "test"), 
		"genos.systime",
		"genos.include",
		"igris",
		("igris.dprint", "stdout"),
		("igris.syslock", "mutex")],
	defines = ["__NOARCH=1"]
)

licant.ex("target")