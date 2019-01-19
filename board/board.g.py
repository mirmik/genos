from licant.modules import module, submodule

module("genos.board", impl = "arduino_mega", 
	sources = ["arduino/board.cpp"],
	include_paths = "arduino",
	defines = ["F_CPU=16000000", "BOARD_ARDUINO_MEGA"],
	mdepends = [
		("genos.hal", "atmega2560"),
	]
)

module("genos.board", impl = "crius_aiop2", 
	sources = ["arduino/board.cpp"],
	include_paths = "arduino",
	defines = ["F_CPU=16000000", "BOARD_CRIUS_AIOP2"],
	mdepends = [
		("genos.hal", "atmega2560"),
	]
)

module("genos.board", impl = "arduino_uno", 
	sources = ["arduino/board.cpp"],
	include_paths = "arduino",
	defines = ["F_CPU=16000000", "BOARD_ARDUINO_UNO"],
	mdepends = [
		("genos.hal", "atmega328p"),
	]
)