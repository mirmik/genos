import licant

module("hal.arm.armv7e-m",
	sources = ["src/nvic.c"],
	include_paths = ["include"],
	mdepends = [
		"hal.arm"
	]
)
