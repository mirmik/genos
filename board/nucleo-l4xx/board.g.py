#module("genos.board.nucleo-l4xx",
#	sources = ["src/board.cpp"],
#	include_paths = "."
#)

#module("genos.board", impl = "nucleo-l412rb", 
#	mdepends = [ "genos.board.nucleo-l4xx", ("genos.hal", "stm32l412") ], 
#	defines=["BOARD_NUCLEO_L412RB"])

#module("genos.board", impl = "nucleo-l432kc", 
#	mdepends = [ "genos.board.nucleo-l4xx", ("genos.hal", "stm32l432") ], 
#	defines=["BOARD_NUCLEO_L432KC"])