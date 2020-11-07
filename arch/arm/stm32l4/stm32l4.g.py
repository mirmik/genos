module("genos.hal.stm32l4.common", "__default__",
	default=True,
	include_paths=["src", "src/cmsis_core"],
	srcdir = "src",

	sources = [
		"stm32l4_vectors_rept.S",
		"stm32l4_start.c",
	#	"stm32l4_start_test.S",
		"stm32l4_arch.c",
		"stm32l4_rcc.c",
		"stm32l4_gpio.c",
		"stm32l4_usart.c",
		"stm32l4_diag.c",
		"stm32l4_spi.c",
	],

	mdepends = [
		"hal.arm.armv7e-m",
		"genos.irqtable",
		"genos.systime"
	],

	defines = ["CHIP_STM32"],

	cc_flags = "-Os -Wl,--gc-sections -nostdinc -fdata-sections -ffunction-sections -Wl,--gc-sections -flto -mthumb -mcpu=cortex-m4 ",
	cxx_flags = "-Os -Wl,--gc-sections -nostdinc -fdata-sections -ffunction-sections -Wl,--gc-sections -flto -fno-rtti -fno-exceptions -mthumb -mcpu=cortex-m4  -fno-threadsafe-statics -ffunction-sections -fno-rtti -flto -fno-use-cxa-atexit",
	ld_flags = "-Os -nostdlib -mthumb -mcpu=cortex-m4 -fno-rtti -fno-exceptions -fdata-sections -ffunction-sections -flto -Wl,--gc-sections -fno-use-cxa-atexit",
	libs = ["m", "gcc"],
	ldscripts = "../stm32/ldscripts/stm32_common.ld",
)

module("genos.hal", impl = "stm32l412", 
	defines = ["CHIP_STM32L412", "CHIP_STM32L412XX"],
	mdepends = [	
		"genos.hal.stm32l4.common",
	],
	ldscripts = "ldscripts/stm32l412.ld",
)
