#!/usr/bin/env python3

import licant 

licant.include("igris", "../../igris.g.py")
licant.include("nos")

licant.cxx_application("target",
	sources=["main.cpp"],
	mdepends = ["igris", "nos"]
)

licant.cxx_application("client",
	sources=["client_main.cpp"],
	mdepends = ["igris", "nos"]
)

licant.ex("target")
licant.ex("client")
