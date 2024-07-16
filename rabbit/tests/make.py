#!/usr/bin/env python3

import sys
import licant
from licant.cxx_modules import application
from licant.modules import submodule, module
from licant.libs import include

licant.execute("../rabbit.g.py")


application("runtests",
		sources = [
			"*.cpp",
			"fitting/*.cpp",
			"geom/*.cpp"
		],

		cxx_flags = "-g -Weffc++ -Wextra -fPIE",
		cc_flags = "-g -Wextra -fPIE",
		ld_flags = "-g -fPIE",
		cxxstd = "c++20",

		include_paths = ["."],
		mdepends = [ "rabbit" ],

		libs=["nos", "ralgo", "igris", 
			#"reactphysics3d"
		],
		defines=["RABBIT_REAL_TYPE=double"]
)

licant.ex("runtests")
