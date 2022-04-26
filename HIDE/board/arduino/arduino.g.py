
module("board.arduino_mega", 
	sources = ["board.cpp"],
	include_paths = ".",
	defines = ["F_CPU=16000000", "BOARD_ARDUINO_MEGA"],
)

module("board.arduino_uno", 
	sources = ["board.cpp"],
	include_paths = ".",
	defines = ["F_CPU=16000000", "BOARD_ARDUINO_UNO"],
)