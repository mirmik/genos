import licant

licant.module("zillot.mock",
	sources=["mock/*.cpp", "mock/*.c"],
	include_paths=["mock/internal"]
)