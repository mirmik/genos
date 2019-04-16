module("genos.hal.stm32.common", 
	include_paths=["include"],
	srcdir = "src",

	sources = [
		"stm32_vectors.S",
		"stm32_start.c",
		"stm32_arch.c",
		"stm32_rcc.c",
		"stm32_diag.c",
	],

	mdepends = [
		"genos.irqtbl",
		"genos.systime"
	],

	cc_flags = "-nostdlib -nostdinc -lgcc -lm -mthumb -mcpu=cortex-m4 -static -fdata-sections -ffunction-sections -Wl,--gc-sections",
	cxx_flags = "-nostdlib -nostdinc -lgcc -lm -mthumb -mcpu=cortex-m4 -static -fdata-sections -ffunction-sections -Wl,--gc-sections",
	ld_flags = "-nostdlib -lgcc -lm -mthumb -mcpu=cortex-m4 -static -fdata-sections -ffunction-sections -Wl,--gc-sections"
)

module("genos.hal", impl = "stm32f407", 
	defines = ["CONFIG_ARCH_CHIP_STM32F407"],
	mdepends = [
		"genos.hal.stm32.common"
	],
	ldscripts = "ldscripts/stm32f401vc.ld",
)
