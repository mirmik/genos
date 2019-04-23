import licant

licant.module("hal.arm",
	srcdir="src",
	sources = ["context.c", "switch_context.S"],
	include_paths = ["include"]
)