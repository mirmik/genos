#!/usr/bin/env python3

import licant
import os

licant.include("genos")

licant.glbfunc.genos_firmware(
	sources=["main.cpp"],
	cxx_flags = "-Wall"
)
