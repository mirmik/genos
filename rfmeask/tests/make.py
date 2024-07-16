#!/usr/bin/env python3

import sys
import licant
from licant.cxx_modules import application
from licant.modules import submodule, module
from licant.libs import include

#licant.include("igris", "../submodules/igris/igris.g.py")
#licant.include("nos", "../submodules/nos/nos.g.py")
#licant.include("ralgo", "../submodules/ralgo/ralgo.g.py")

application("runtests",
            sources=[
                "main.cpp",
                "nd/*.cpp",
            ],

            #		"../src/scriptexec.cpp",
            #		"../src/tables.cpp",
            #		"../src/GearSettings.cpp",
            #		"../src/MitsubishiServo.cpp",
            #		"../src/MitsubishiCommunicator.cpp",
            #		"../src/AbstractServo.cpp",
            #		"../src/Device.cpp",
            #		"../src/logging.cpp",
            #		"../src/Notifier.cpp",
            #		"../src/NotifyBrocker.cpp",
            #		"../src/NotifyTheme.cpp",
            #		"../src/DevAlarmLogger.cpp",
            #		"../src/parameters_util.cpp",
            #		"../src/MitsuServoAParameter.cpp"


            cxxstd="c++20",
            cxx_flags="-g -fdata-sections -ffunction-sections -Wl,--gc-sections",
            cc_flags="-g -fdata-sections -ffunction-sections -Wl,--gc-sections",
            ld_flags="-fdata-sections -ffunction-sections -Wl,--gc-sections",

            include_paths=[
                ".",
                "../lib/libscpi/inc/",
                "../src",
                "../submodules/nos",
                "../submodules/crow",
            ],

            mdepends=[],

            libs=["pthread", "igris", "nos", "ralgo"]
            )

licant.ex("runtests")
