#!/usr/bin/env python3

import licant
import os

licant.include("genos")

licant.glbfunc.genos_firmware(
	mdepends=["igris.stdlibs"],
	sources=["main.cpp"]
)
