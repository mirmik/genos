#!/usr/bin/env python3.5
#coding: utf-8

import glink.cxx_modules
from glink.modules import module, submodule

glink.scripter.scriptq.execute("../../genos.g.py")

avr_binutils = glink.cxx_make.binutils(
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

glink.cxx_modules.make("main")
glink.make.doit("build/genos")