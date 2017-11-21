module("genos.board", "native", 
	sources = ["src/board.c"],
	include_paths = "include",
	include_modules = [submodule("genos.hal", "native")]
)
