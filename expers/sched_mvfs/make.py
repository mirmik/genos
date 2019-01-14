#!/usr/bin/env python3
#coding:utf-8

import licant
import licant.libs
from licant.cxx_modules import application

licant.libs.include("genos")
licant.libs.include("gxx")

application("target",
	sources = ["main.cpp"],

	mdepends = [ 
		("gxx", "posix"),
		("gxx.dprint", "stdout"),
		("gxx.syslock", "mutex"),

		"genos.include",
		"genos.mvfs",
		"genos.mvfs.schedee_support",
		
		("genos.sched.cooperative", "__none__"),
		("genos.sched", "impl"),
	],

	cc_flags = "-std=gnu11"
)

licant.ex("target")