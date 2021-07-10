import licant

licant.module("hal.arm",
	srcdir="src",
	include_paths = ["src"],
	sources = ["context.c", "switch_context.S", "delay.c", "hardfault.c"],
)


module("cpu.arm.cortex-m4",
	mdepends=["hal.arm", "arch.cortex-m4"]	
)