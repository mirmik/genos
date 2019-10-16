import licant

licant.module("hal.arm",
	srcdir="src",
	include_paths = ["src"],
	sources = ["context.c", "switch_context.S", "delay.c", "hardfault.c"],
)