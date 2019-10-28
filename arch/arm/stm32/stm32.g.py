import licant

module("genos.hal.stm32.common", "base",
	default=True,
	include_paths=["src"],
	srcdir = "src",

	sources = [
		"stm32_vectors_rept.S",
		"stm32_start.c",
		"stm32_arch.c",
		"stm32_usart.c",
		"stm32_gpio.c",
		"stm32_rcc.c",
		"stm32_adc.c",
		"stm32_spi.c",
		"stm32_diag.c",
	],

	mdepends = [
		"genos.irqtable",
		"genos.systime"
	],

	defines = ["CHIP_STM32"],

	cc_flags = "-Os -Wl,--gc-sections -nostdinc -fdata-sections -ffunction-sections -Wl,--gc-sections -flto -mthumb -mcpu=cortex-m4 ",
	cxx_flags = "-Os -Wl,--gc-sections -nostdinc -fdata-sections -ffunction-sections -Wl,--gc-sections -flto -fno-rtti -fno-exceptions -mthumb -mcpu=cortex-m4  -fno-threadsafe-statics -ffunction-sections -fno-rtti -flto -fno-use-cxa-atexit",
	ld_flags = "-Os -nostdlib -mthumb -mcpu=cortex-m4 -fno-rtti -fno-exceptions -fdata-sections -ffunction-sections -flto -Wl,--gc-sections -fno-use-cxa-atexit",
	libs = ["m", "gcc"],
	ldscripts = "ldscripts/stm32_common.ld",
)

module("genos.hal.stm32.common", "startup_debug",
	include_paths=["src"],
	srcdir = "src",

	sources = [
		"stm32_vectors_rept.S",
		#"stm32_start.c",
		"stm32_arch.c",
		"stm32_usart.c",
		"stm32_gpio.c",
		"stm32_rcc.c",
		"stm32_adc.c",
		"stm32_spi.c",
		"stm32_diag.c",
	],

	defines = ["CHIP_STM32"],

	cc_flags = "-Os -Wl,--gc-sections -nostdinc -fdata-sections -ffunction-sections -Wl,--gc-sections -flto -mthumb -mcpu=cortex-m4 ",
	cxx_flags = "-Os -Wl,--gc-sections -nostdinc -fdata-sections -ffunction-sections -Wl,--gc-sections -flto -fno-rtti -fno-exceptions -mthumb -mcpu=cortex-m4  -fno-threadsafe-statics -ffunction-sections -fno-rtti -flto -fno-use-cxa-atexit",
	ld_flags = "-Os -nostdlib -mthumb -mcpu=cortex-m4 -fno-rtti -fno-exceptions -fdata-sections -ffunction-sections -flto -Wl,--gc-sections -fno-use-cxa-atexit",
	libs = ["m", "gcc"]
)


module("genos.hal.stm32f4xx", 
	mdepends = [
		"hal.arm.armv7e-m",
		#"hal.arm"
	]
)

module("genos.hal", impl = "stm32f407", 
	defines = ["CHIP_STM32F407"],
	mdepends = [
		"genos.hal.stm32f4xx",	
		"genos.hal.stm32.common",
	],
	ldscripts = "ldscripts/stm32f407.ld",
)

module("genos.hal", impl = "stm32f401", 
	defines = ["CHIP_STM32F401"],
	mdepends = [
		"genos.hal.stm32f4xx",	
		"genos.hal.stm32.common",
	],
	ldscripts = "ldscripts/stm32f401re.ld",
)

module("genos.stm32.spl.stm32f4xx", 
	srcdir = "./src/spl",
	sources = [
		"stm32f4xx_tim.c"
	]

	#os.listdir(os.path.join(licant.directory(), "src/spl")),
	#sources = licant.util.find_recursive(
	#	root=os.path.join(licant.directory(), "src/spl"),
	#	pattern=".c",
	#	hide="HIDE",
	#	debug=True
	#)
)
