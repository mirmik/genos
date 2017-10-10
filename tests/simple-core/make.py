#!/usr/bin/env python3.5
#coding: utf-8

import licant.cxx_modules
from licant.modules import module, submodule

licant.scripter.scriptq.execute("../../genos.g.py")

avr_binutils = licant.cxx_make.binutils(
	cxx = "avr-g++", 
	cc = "avr-gcc", 
	ld = "avr-ld", 
	ar = "avr-ar", 
	objdump = "avr-objdump"
)

module("main",
	target = "build/genos",
	type = "application",
	binutils = avr_binutils,

	cxxstd = "gnu++14",

	sources = ["main.c"],
	include_modules = [
		submodule("genos.include"),
		submodule("genos.board", "arduino_mega"),
		submodule("genos.scheduler"),

		submodule("gxx"),
		submodule("gxx.libc"),
		submodule("gxx.cxxstd"),
		submodule("gxx.dprint", "diag"),
		submodule("gxx.diag", "impl"),

		submodule("genos.irqtbl"),
		submodule("genos.atomic", "irqs")
	]
)

licant.cxx_modules.make("main")
licant.make.doit("build/genos")