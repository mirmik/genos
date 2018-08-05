from licant.modules import module, submodule

module("genos.board", impl = "arduino_uno", 
	sources = ["board.cpp"],
	srcdir = "src",
	include_paths = "include",
	defines = ["F_CPU=16000000"],
	include_modules = [submodule("genos.hal", impl="atmega328p")]
)