#!/usr/bin/env python3
#coding:utf-8

import licant
import licant.libs
from licant.cxx_modules import application

licant.libs.include("genos")
licant.libs.include("igris")

application("target",
	sources = ["main.c"],

	mdepends = [
		"genos.include", 
		"igris.include", 

		("igris.dprint", "stdout"),
		"igris.bug",

		"genos.mvfs",
		"genos.mvfs.global",
		
		("genos.sched", "stub"),
		("genos.errno"),

	],
)

licant.ex("target")