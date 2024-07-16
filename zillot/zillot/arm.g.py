import licant

licant.module("zillot.arm",
	srcdir="arm",
	include_paths = ["arm/internal"],
	sources = [
		"internal/asm/context.c", 
		"internal/asm/switch_context.S", 
		"internal/asm/delay.c", 
		"arm_hardfault.c"],
)

licant.module("zillot.armv7e-m",
	sources = ["armv7e-m/arm_nvic.c"],
	include_paths = ["armv7e-m/internal"],
	mdepends = [
		"zillot.arm"
	]
)

licant.module("zillot.cortex-m4",
	mdepends = ["zillot.armv7e-m"],
	include_paths=["cortex-m4"]
)

licant.module("zillot.cortex-m7",
	mdepends = ["zillot.armv7e-m"],
	include_paths=["cortex-m7"]
)

