#!/usr/bin/env python3

import licant

licant.include("rabbit", "../../rabbit.g.py")
licant.include("igris")

licant.cxx_application("target",
	mdepends=["rabbit", "rabbit.opengl", "igris.util", "rabbit.mesh"],
	sources = ["main.cpp"],
	libs = ["GLEW", "glfw", "GL", "GLU", "X11", "pthread", "Xrandr", "Xi", "SOIL", "igris", "nos"],
	include_paths=["/usr/include/SOIL"],
	cxx_flags = "-g"
)

licant.ex("target")