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
		"gxx.c_only",
		("gxx.dprint", "cout"),
		("gxx.syslock", "mutex"),

		"genos.include",
		"genos.mvfs",
		"genos.mvfs.schedee_support",
		
		("genos.sched", "impl"),
	],

	cc_flags = "-std=gnu11"
)

licant.ex("target")