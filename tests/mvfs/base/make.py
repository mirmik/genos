#!/usr/bin/python3
#coding:utf-8

import licant
import licant.libs
from licant.cxx_modules import application

licant.libs.include("genos")
licant.libs.include("gxx")

application("target",
	sources = ["main.c"],

	include_modules = [
		"genos.include", 
		("gxx", "posix"),
		("gxx.print", "cout"),
		("gxx.dprint", "cout"),

		"genos.mvfs",
		"genos.mvfs.global",
		
		("genos.sched", "stub"),
		("genos.errno"),
		#"genos.misc",
		#"genos.chardev"
	],

	cc_flags = "-std=gnu11"
)

licant.ex("target")