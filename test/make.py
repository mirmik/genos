#!/usr/bin/env python3

import sys
import licant
from licant.cxx_modules import application
from licant.modules import submodule, module
from licant.libs import include

licant.execute("../genos.g.py")

targets = []

application("runtests",
	sources = [
		"main.cpp",
		"drivers_gpio_devices.cpp",
	],
	cxx_flags = "-g",
	cc_flags = "-g",
	include_paths = ["."],
	mdepends = [ "igris", "genos.include" ],
)
targets.append("runtests")

def avr_application(chip, board, target):
	application(target+"_firmware",
		toolchain=licant.cxx_make.toolchain_gcc("avr-"),
		builddir="build_"+target,
		sources=[
			"avr_main.cpp"
		],
		mdepends=[
			"genos.include",
			"genos.systime",
			"genos.sched",

			board,
			chip,
	
			("igris.dprint", "diag"),
			"igris.bug",
			"igris.cxx_support",
			("igris.syslock", "genos.atomic"),
	
			"drivers.avr.gpio",
		]
	)
	targets.append(target+"_firmware")

def stm32_application(chip, board, target):
	application(target+"_firmware",
		toolchain=licant.cxx_make.toolchain_gcc("arm-none-eabi-"),
		builddir="build_"+target,
		sources=[
			"stm32_main.cpp"
		],
		mdepends=[
			"genos.include",
			"genos.irqtable",
			"genos.systime",
			"genos.newlib_bind",
			"genos.sched",
		
			chip,
			board,

			("igris.dprint", "diag"),
			"igris.bug",
			"igris.cxx_support",
			("igris.syslock", "genos.atomic")
		]
	)
	targets.append(target+"_firmware")
	
stm32_application(board="board.nucleo-f401re", chip="cpu.stm32.stm32f401re", target="stm32f401re")
stm32_application(board="board.nucleo-g474re", chip="cpu.stm32.stm32g474re", target="stm32g474re")
stm32_application(board="board.nucleo-g431rb", chip="cpu.stm32.stm32g431rb", target="stm32g431rb")

avr_application(board="board.arduino_mega", chip="cpu.avr.atmega2560", target="arduino_mega_2560")
avr_application(board="board.arduino_uno", chip="cpu.avr.atmega328p", target="arduino_uno_328p")

licant.fileset("all", targets)

licant.ex("all")

