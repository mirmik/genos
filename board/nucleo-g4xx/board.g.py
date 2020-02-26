module("board.nucleo-g4xx",
	sources = ["board.cpp"],
	include_paths = "."
)

module("board.nucleo-g431rb", 
	mdepends = [ "board.nucleo-g4xx", "arch.stm32g431rb" ], 
	defines=["BOARD_NUCLEO_G431RB"])
