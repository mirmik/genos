import licant

licant.module("hal.arm",
	srcdir="src",
	sources = ["context.c", "switch_context.S", "delay.c"],
	include_paths = ["include"]
)