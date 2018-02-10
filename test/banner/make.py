#!/usr/bin/env python3

from licant.cxx_modules import application, doit
from licant.modules import submodule
from licant.libs import include

include("gxx")
include("genos")

application("target",
	sources = ["main.cpp"],
	include_paths = ["."],

	include_modules = [
		submodule("genos.base"),
	]
)

doit("target")