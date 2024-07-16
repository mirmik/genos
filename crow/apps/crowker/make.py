#!/usr/bin/env python3
#coding: utf-8

import licant
import licant.install
from licant.cxx_modules import application
from licant.libs import include
import os

licant.include("crow")

application("crowker",
            sources=[
                "main.cpp",
                "control_node.cpp"
            ],
            mdepends=[
                "crow",
                "crow.crowker",
                "crow.udpgate"
            ],
            optimize="-O0",
            cc_flags="-flto -fPIC -Wextra -Wall -ffunction-sections -fdata-sections",
            cxx_flags="-flto -fPIC -Wextra -Wall -ffunction-sections -fdata-sections",
            ld_flags="-flto -fPIC -static -Wl,--whole-archive -lpthread -Wl,--no-whole-archive",
            libs=["readline", "nos", "igris"]
            )

licant.install.install_application(tgt="install_crowker", src="crowker", dst="crowker")

if __name__ == "__main__":
    licant.install.install_application(tgt="install", src="crowker", dst="crowker")
    licant.ex("crowker")
