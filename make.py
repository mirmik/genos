#!/usr/bin/env python3
#coding: utf-8

import licant
import licant.install
import os

licant.execute("genos.g.py")
licant.include("zillot")
licant.include("igris")

licant.cli.add_argument("--toolchain")
opts, args = licant.cli.parse()
toolchain = licant.gcc_toolchain(opts.toolchain)

target = lambda suffix: "libgenos.{}".format(suffix)

modules = [
		"genos",
		"genos.host_stub",
		"zillot.include"
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

licant.cxx_application("runtests",
                       sources=["tests/*.cpp"],

                       include_paths=[".", "tests"],
                       mdepends=[
                           "genos",
                           "zillot",
                           "zillot.mock",
                           "igris.include",
                           ("igris.systime", "jiffies"),
                       ],
                       libs=["nos", "igris"],

                       cxx_flags="-flto -ffunction-sections -fexceptions -fdata-sections -Wl,--gc-sections -g -fno-rtti",
                       cc_flags="-flto -ffunction-sections -fdata-sections -Wl,--gc-sections -g",
                       ld_flags="-flto -ffunction-sections -fdata-sections -Wl,--gc-sections -g -fno-rtti",
                       )

licant.fileset("all", ["runtests", "libgenos.so", "libgenos.a"])

licant.install.install_library(
	tgt="install",
	uninstall="uninstall",
	libtgt="libgenos.so",
	hroot="genos",
	headers="genos")

licant.ex("all")
