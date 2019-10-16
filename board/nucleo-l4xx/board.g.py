module("genos.board.nucleo-l4xx",
	sources = ["src/board.cpp"],
	include_paths = "."
)

module("genos.board", impl = "nucleo-l412rb", 
	mdepends = [ "genos.board.nucleo-l4xx", ("genos.hal", "stm32l412") ], 
	defines=["BOARD_NUCLEO_L412RB"])