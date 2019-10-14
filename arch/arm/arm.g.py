import licant

licant.module("hal.arm",
	srcdir="src",
	sources = ["context.c", "switch_context.S", "dwt.c"],
	mdepends = [("systime.delayMicroseconds", "dwt_delay")],
	include_paths = ["include"]
)