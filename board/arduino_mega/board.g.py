from glink.modules import module, submodule

module("genos.board", impl = "arduino_mega", 
	sources = ["board.c"],
	srcdir = "src",
	include_paths = "include",
	defines = ["F_CPU=16000000"],
	include_modules = [submodule("genos.hal", impl="atmega2560")]
)