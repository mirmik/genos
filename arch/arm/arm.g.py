import licant

licant.module("hal.arm",
	srcdir="src",
	sources = ["context.c", "switch_context.S", "dwt.c", "delay.c"],
	include_paths = ["include"]
)