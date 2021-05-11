module("board.nucleo",
	sources = ["src/board.cpp"],
	include_paths = [".", "include"]
)

module("board.nucleo-f401re",
	mdepends = [
		"board.nucleo", 
#		"arch.stm32f401re",
	],
	defines=["BOARD_NUCLEO_F401RE"]
)

module("board.nucleo-g431rb",
	mdepends = [
		"board.nucleo", 
#		"arch.stm32g431rb",
	],
	defines=["BOARD_NUCLEO_G431RB"]
)

module("board.nucleo-f446re",
	mdepends = [
		"board.nucleo", 
#		"arch.stm32f446re",
	],
	defines=["BOARD_NUCLEO_F446RE"]
)

module("board.nucleo-g474re", 
	mdepends = [ 
		"board.nucleo", 
#		"arch.stm32g474re"
	], 
	defines=["BOARD_NUCLEO_G474RE"]
)

module("board.nucleo-g474.init", 
	mdepends = [ 
		"board.nucleo",
	], 
	defines=["BOARD_NUCLEO_G474RE"]
)

module("genos.board", impl = "nucleo-l412rb", 
	mdepends = [ "board.nucleo", ("genos.hal", "stm32l412") ], 
	defines=["BOARD_NUCLEO_L412RB"])

module("genos.board", impl = "nucleo-l432kc", 
	mdepends = [ "board.nucleo", ("genos.hal", "stm32l432") ], 
	defines=["BOARD_NUCLEO_L432KC"])