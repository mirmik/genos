#!/usr/bin/env python3 

import licant
import licant.install

licant.cxx_application("dataproxy",
	srcdir = "src",
	sources = [
		"main.cpp", 
		"signal-analyzer-harvest.cpp",
		"buffered_channels.cpp"
	],
	include_paths = ["src"],
	mdepends = [],
	libs=["pthread", "nos", "igris"],
	cc_flags = "-g",
	cxx_flags = "-g",
	ld_flags = "-static -Wl,--whole-archive -lpthread -Wl,--no-whole-archive -g",
)

licant.install.install_application(tgt="install", src="dataproxy", dst="")

licant.ex("dataproxy")
