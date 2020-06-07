#module("board.nucleo-g4xx",
#	sources = ["board.cpp"],
#	include_paths = "."
#)
#
#module("board.nucleo-g474re", 
#	mdepends = [ 
#		"board.nucleo-g4xx", 
#		"arch.stm32g474re"
#	], 
#	defines=["BOARD_NUCLEO_G474RE"])
#