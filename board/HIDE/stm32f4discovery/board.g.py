module("genos.board", impl = "stm32f4discovery",
	sources = ["src/board.c"],
	include_paths = "include",
	mdepends = [
		("genos.hal", "stm32f401"),
	]	
)