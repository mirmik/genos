
module("board.arduino_mega", 
	sources = ["board.cpp"],
	include_paths = ".",
	defines = ["F_CPU=16000000", "BOARD_ARDUINO_MEGA"],
)


module("board.arduino_nano_168", 
	sources = ["board.cpp"],
	include_paths = ".",
	defines = ["F_CPU=16000000", "BOARD_ARDUINO_NANO"],
)