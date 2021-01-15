module("arch.stm32f7_common.sources", 	
	sources= [
		"src/stm32_start.c",
		"src/stm32_vectors.S",
		"src/stm32_arch.c",
	#	"src/stm32_adc.c",
		"src/stm32_rcc.c",
		"src/stm32_gpio.c",
		"src/stm32_spi.c",
#		"src/stm32_i2c.c",
		"src/stm32_usart.c",
		"src/stm32_diag.c",
		"src/stm32_systick.c",
		"src/stm32_clockbus.c",
#		"src/stm32_pll.c",
	],
)

module("arch.stm32f7_common",
	include_paths = ["include", "st-device"],

	ldscripts= ["ldscripts/stm32_common.ld"],

	cc_flags = "-Os -g -Wl,--gc-sections -nostdlib -nostdinc -fdata-sections -ffunction-sections -Wl,--gc-sections -flto -mthumb -mcpu=cortex-m7 ",
	cxx_flags = "-Os -g -Wl,--gc-sections -nostdlib -nostdinc -fdata-sections -ffunction-sections -Wl,--gc-sections -flto -fno-rtti -fno-exceptions -mthumb -mcpu=cortex-m7  -fno-threadsafe-statics -ffunction-sections -fno-rtti -flto -fno-use-cxa-atexit",
	ld_flags = "-Os -g -nostdlib -mthumb -mcpu=cortex-m7 -fno-rtti -fno-exceptions -fdata-sections -ffunction-sections -flto -Wl,--gc-sections -fno-use-cxa-atexit",

	libs = ["m", "gcc"]
)

module("arch.stm32f7",
	defines=["STM32F7XX", "CHIP_STM32F7XX"],
	sources=["src/stm32f7/stm32f7_sysinit.c"],
	mdepends = [
		"hal.arm.armv7e-m",
		"cmsis-base", 
		"arch.stm32f7_common", 
		"arch.stm32f7_common.sources"]
)