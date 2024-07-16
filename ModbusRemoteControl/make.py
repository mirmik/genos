#!/usr/bin/env python3

import os
import licant
import licant.install
import subprocess

licant.cxx_application("ModbusRemoteControl",
                       sources=[
                           "*.cpp"
                       ],
                       include_paths=["."],
                       libs=["nos", "igris", "modbus", "pthread"],
                       cxx_flags="-pedantic-errors --static -Werror=all -Werror=extra -g",
                       ld_flags="-L /usr/lib/x86_64-linux-gnu/ --static"
                       )

licant.install.install_application(
    tgt="install", src="ModbusRemoteControl", dst="ModbusRemoteControl")

licant.ex("ModbusRemoteControl")
