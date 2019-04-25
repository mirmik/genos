#!/usr/bin/python3
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

		"genos.mvfs",
		"genos.mvfs.global",
		#"genos.misc",
		#"genos.chardev"

		("igris.dprint", "stdout"),
		"igris.bug"
	],

	cc_flags = "-std=gnu11"
)

licant.ex("target")