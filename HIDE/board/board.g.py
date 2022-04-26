from licant.modules import module, submodule

module("genos.board", impl = "arduino_mega", 
	sources = ["arduino/board.cpp"],
	include_paths = "arduino",
	defines = ["F_CPU=16000000", "BOARD_ARDUINO_MEGA"],
	mdepends = [
		("genos.hal", "atmega2560"),
	]
)

module("genos.board", impl = "arduino_nano_328", 
	sources = ["arduino/board.cpp"],
	include_paths = "arduino",
	defines = ["F_CPU=16000000", "BOARD_ARDUINO_NANO"],
	mdepends = [
		("genos.hal", "atmega328"),
	]
)

module("genos.board", impl = "arduino_nano_168", 
	sources = ["arduino/board.cpp"],
	include_paths = "arduino",
	defines = ["F_CPU=16000000", "BOARD_ARDUINO_NANO"],
	mdepends = [
		("genos.hal", "atmega168"),
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

module("board.rl.syncro_v2",
	sources = ["rl/syncro_v2/board.cpp"],
	include_paths = "rl/syncro_v2",
	mdepends = [
		"arch.stm32f407vg",
		"genos.drivers.stm32",
	],
	defines=["BOARD_RL_SYNCRO_V2"]
)