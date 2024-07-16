#!/usr/bin/env python3

import licant

licant.cxx_application("listener",
    sources=["listener.cpp"],
    libs = ["nos", "pthread"]
)

licant.ex("listener")


licant.cxx_application("beam",
    sources=["beam.cpp"],
    libs = ["nos", "pthread"],
    builddir="build2"
)

licant.ex("beam")
