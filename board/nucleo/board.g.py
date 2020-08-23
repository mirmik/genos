module("board.nucleo",
	sources = ["src/board.cpp"],
	include_paths = "."
)

module("board.nucleo-f401re",
	mdepends = [
		"board.nucleo", 
		("genos.hal", "stm32f401"),
	],
	include_paths=["nucleo-f401re"],
	defines=["BOARD_NUCLEO_F401RE"]
)

module("board.nucleo-f446re",
	mdepends = [
		"board.nucleo", 
		"arch.stm32f446re",
	],
	include_paths=["nucleo-f446re"],
	defines=["BOARD_NUCLEO_F446RE"]
)

module("board.nucleo-g474re", 
	mdepends = [ 
		"board.nucleo", 
		"arch.stm32g474re"
	], 
	include_paths=["nucleo-g474re"],
	defines=["BOARD_NUCLEO_G474RE"]
)
