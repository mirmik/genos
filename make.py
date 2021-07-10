#!/usr/bin/env python3
#coding: utf-8

import licant
import licant.install
import os

licant.execute("genos.g.py")

licant.cli.add_argument("--toolchain")
opts, args = licant.cli.parse()
toolchain = licant.cxx_make.toolchain_gcc(opts.toolchain)

target = lambda suffix: "libgenos.{}".format(suffix)

modules = [
		"genos",
	]

CCFLAGS = '-fPIC -Wall -pedantic-errors -Wreturn-type -Wno-gnu-zero-variadic-macro-arguments'
CXXFLAGS = CCFLAGS

licant.cxx_library("shared",
	target="libgenos.so",
	toolchain=toolchain,
	mdepends = modules,
	cxx_flags = CXXFLAGS,
	cc_flags = CCFLAGS,
	shared = True,
	cxxstd = "gnu++17",
	ccstd = "gnu11",
	optimize = "-O3"
)

licant.cxx_library("static",
	target="libgenos.a",
	toolchain=toolchain,
	mdepends = modules,
	cxx_flags = CXXFLAGS,
	cc_flags = CCFLAGS,
	cxxstd = "gnu++17",
	ccstd = "gnu11",
	optimize = "-O3",
	shared = False
)

licant.install.install_library(
	tgt="install",
	uninstall="uninstall",
	libtgt="libgenos.so",
	hroot="genos",
	headers="genos")

licant.ex("shared")
