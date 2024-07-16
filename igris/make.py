#!/usr/bin/env python3
# coding: utf-8

import licant
import licant.install
import os
import sys

licant.execute("igris.g.py")

licant.cli.add_argument("--igris-std", action="store_true")
opts, args = licant.cli.parse()


def target(suffix):
    return "libigris.{}".format(suffix)


DEFINES = []
c_flags = ""
cxx_flags = ""
modules = [
    "igris",
    "igris.dprint",
    "igris.protocols.gstuff",
    "igris.series",
]

stdmodules = []

if sys.platform == "linux" and not opts.igris_std:
    modules.extend([
        ("igris.os_extension", "unix"),
        "igris.sclonner"
    ])

if sys.platform == "win32" and not opts.igris_std:
    modules.extend([
        ("igris.os_extension", "windows"),
        "igris.sclonner"
    ])

if opts.igris_std:
    c_flags = c_flags + "-nodefaultlibs -nostdlib -nostdinc "
    cxx_flags = cxx_flags + "-nodefaultlibs -nostdlib -nostdinc -nostdinc++ "
    stdmodules.append("igris.compat.std")
    stdmodules.append("igris.compat.libc")
    stdmodules.append("igris.compat.posix")
    stdmodules.append("igris.compat.stubarch")
    # determine wordsize
    if os.uname().machine == "x86_64":
        wordsize = 64
    elif os.uname().machine == "i686":
        wordsize = 32
    else:
        raise Exception("Unknown wordsize")
    DEFINES.extend([f"IGRIS_STUBARCH_WORDSIZE={wordsize}"])

CCFLAGS = '-fPIC -Werror=all -Werror=extra -pedantic-errors -Wreturn-type -g -Wno-gnu-zero-variadic-macro-arguments'
CXXFLAGS = cxx_flags + CCFLAGS
CCFLAGS = c_flags + CCFLAGS
LDFLAGS = '-fPIC -g -flto'

licant.cxx_static_and_shared(
    name="libraries",
    static_lib="libigris.a",
    shared_lib="libigris.so",
    mdepends=stdmodules+modules + [
        "igris.syslock",
        ("igris.ctrobj", "linux"),
        ("igris.platform", "host"),
        "igris.printf_impl",
    ],
    defines=DEFINES,
    cxx_flags=CXXFLAGS,
    cc_flags=CCFLAGS,
    ld_flags=LDFLAGS,
    cxxstd="c++17",
    ccstd="c11",
    optimize="-O3"
)

licant.cxx_application("runtests",
                       sources=[
                           "tests/*.cpp",
                           "tests/container/*.cpp",
                           "tests/shell/*.cpp",
                           "tests/series/*.cpp",
                           "tests/archive/*.cpp",
                       ],
                       defines=DEFINES,
                       objects=["libigris.a"],
                       cxxstd="c++20",
                       ccstd="c11",
                       cxx_flags=cxx_flags +
                       "-fPIC -fmax-errors=1 -g -Werror=all -Wno-gnu-zero-variadic-macro-arguments -Weffc++",
                       cc_flags=c_flags + "-g -Werror=all -Wno-gnu-zero-variadic-macro-arguments",
                       ld_flags="-fPIC -g",
                       include_paths=["./tests", "."],
                       libs=["rt", "pthread"],
                       mdepends=stdmodules
                       )

licant.install.install_library(
    tgt="install",
    uninstall="uninstall",
    libtgt=["libigris.so", "libigris.a"],
    hroot="igris",
    headers="igris")

licant.fileset("all", targets=["runtests", "libigris.so", "libigris.a"])

licant.ex("all")
