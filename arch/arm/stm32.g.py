module("genos.hal.stm32.common", 
	include_paths=["include"],
	srcdir = "src/stm32",

	sources = [
		"stm32_vectors.S",
		"stm32_start.c",
		"stm32_arch.c",
		"stm32_usart.c",
		"stm32_gpio.c",
		"stm32_rcc.c",
		"stm32_diag.c",
	],

	mdepends = [
		"genos.irqtbl",
		"genos.systime"
	],

	defines = ["CHIP_STM32"],

	cc_flags = "-nostdlib -nostdinc -mthumb -mcpu=cortex-m4 -static -fdata-sections -ffunction-sections -Wl,--gc-sections",
	cxx_flags = "-nostdlib -nostdinc -fno-rtti -mthumb -mcpu=cortex-m4 -static -fdata-sections -ffunction-sections -Wl,--gc-sections",
	ld_flags = "-mthumb -mcpu=cortex-m4 -nostdlib -static -fdata-sections -ffunction-sections -Wl,--gc-sections"
)

module("genos.hal.stm32f4xx", 
	mdepends = [
		"hal.arm.armv7e-m",
		"hal.arm"
	]
)

module("genos.hal", impl = "stm32f407", 
	defines = ["CHIP_STM32F407"],
	mdepends = [
		"genos.hal.stm32f4xx",	
		"hal.arm32.include",
		"genos.hal.stm32.common",
	],
	ldscripts = "ldscripts/stm32f401vc.ld",
)

module("genos.hal", impl = "stm32f401", 
	defines = ["CHIP_STM32F401"],
	mdepends = [
		"genos.hal.stm32f4xx",	
		"genos.hal.stm32.common",
	],
	ldscripts = "ldscripts/stm32f401vc.ld",
)
