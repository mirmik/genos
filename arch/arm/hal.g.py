from licant.modules import module, submodule

module("genos.hal.arm.common",
	include_paths = ["include"],
	sources = ["src/regops.S", "src/start.c", "src/vectors.S"],

	cxx_flags = "-nostdlib  -nostdinc -lgcc -lm -march=armv6-m -mthumb -Wl,--gc-sections -fdata-sections -ffunction-sections -fno-rtti",
	cc_flags = "-nostdlib -nostdinc -lgcc -lm -march=armv6-m -mthumb -Wl,--gc-sections -fdata-sections -ffunction-sections",
	ld_flags = "-nostdlib -lgcc -lm -march=armv6-m -mthumb -Wl,--gc-sections -fdata-sections -ffunction-sections",
)

module("genos.hal.arm.arch", impl = "armv6-m",
	include_paths = ["arch/armv6-m/include"],
	mdepends = [("genos.hal.arm.common")]	
)

module("genos.hal.arm.arch", impl = "armv7-m",
	include_paths = ["arch/armv6-m/include"],	
	mdepends = [("genos.hal.arm.common")]	
)

module("genos.hal.arm.family", impl = "cortex-m0+",
	mdepends = [("genos.hal.arm.arch", "armv6-m")]	
)

module("genos.hal.chip", impl = "samd21g18", 
	include_paths = ["chip/samd21g18/include"],
	mdepends = [("genos.hal.arm.family", "cortex-m0+")],
	ldscripts = "chip/samd21g18/ldscript.ld",
)

module("genos.hal.arm.stm32", 
	include_paths=["stm32/include"],
	srcdir = "stm32/src",
	sources = [
		"stm32_arch.c",
		"stm32_rcc.c",
		"stm32_diag.c",
	]
)

module("genos.hal", impl = "stm32f407", 
	#include_paths = ["chip/samd21g18/include"],
	mdepends = [
		("genos.hal.arm.family", "cortex-m0+"),
		"genos.hal.arm.stm32"
	],
	ldscripts = "ldscripts/stm32f401vc.ld",
)


#module("genos.hal.avr.common",
#	sources = [
#		"src/asm/arch.cpp",
#		"src/asm/diag.c",
#		"src/asm/systime.cpp",

#		"src/drivers/gpio.c",
#		"src/drivers/timer.c",
#		"src/asm/atmega/*"
#	],
#	include_paths = ["include"]
#)

#module("genos.hal.chip", impl = "arm_m0",
#	include_paths = ["atmega2560/include"],
#	mdepends = [submodule("genos.hal.avr.common")],
#	ldscripts = ["atmega2560/ldscripts/flash.ld"],

#	defines = ["CHIP_ATMEGA2560"],
#	cxx_flags = "-nostdlib -lgcc -lm -mmcu=atmega2560 -Wl,--gc-sections -fdata-sections -ffunction-sections -fno-rtti -mmcu=atmega2560",
#	cc_flags = "-nostdlib -lgcc -lm -mmcu=atmega2560 -Wl,--gc-sections -fdata-sections -ffunction-sections -mmcu=atmega2560",
#	ld_flags = "-nostdlib -lgcc -lm -mmcu=atmega2560 -Wl,--gc-sections -fdata-sections -ffunction-sections",
#)
