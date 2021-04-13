module("board.stm32f746g-disco",
	sources=["curboard.cpp"],
	mdepends=["arch.stm32f746ng"],
	include_paths=["."]
)