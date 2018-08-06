from licant.modules import module, submodule

module("genos.board", impl = "arduino_mega", 
	sources = ["arduino/board.cpp"],
	include_paths = "arduino",
	defines = ["F_CPU=16000000", "BOARD_ARDUINO_MEGA"],
	include_modules = [submodule("genos.hal", impl="atmega2560")]
)

module("genos.board", impl = "arduino_uno", 
	sources = ["arduino/board.cpp"],
	include_paths = "arduino",
	defines = ["F_CPU=16000000", "BOARD_ARDUINO_UNO"],
	include_modules = [submodule("genos.hal", impl="atmega328p")]
)