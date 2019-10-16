module("genos.board.nucleo-x4xx",
	sources = ["src/board.cpp"],
	include_paths = "."
)

module("genos.board", "nucleo-l432",
	mdepends=[
		"genos.board.nucleo-x4xx", 
		("genos.hal", "stm32l432")
	],
	defines=["BOARD_NUCLEO_L432"]
)

module("genos.board", impl = "nucleo-f401re",
	sources = ["src/board.cpp"],
	include_paths = ".",
	mdepends = [
		("genos.hal", "stm32f401"),
	],
	defines=["BOARD_NUCLEO_F401RE"]
)