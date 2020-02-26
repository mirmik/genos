import licant.modules

module("arch.stm32_common",
	include_paths = ["include", "st-device"],

	sources= [
		"src/stm32_start.c",
		"src/stm32_vectors.S",

		"src/stm32_arch.c",

		"src/stm32_rcc.c",
		"src/stm32_gpio.c",
	],
	ldscripts= ["ldscripts/stm32_common.ld"],

	cc_flags = "-O0 -g -Wl,--gc-sections -nostdinc -fdata-sections -ffunction-sections -Wl,--gc-sections -flto -mthumb -mcpu=cortex-m4 ",
	cxx_flags = "-O0 -g -Wl,--gc-sections -nostdinc -fdata-sections -ffunction-sections -Wl,--gc-sections -flto -fno-rtti -fno-exceptions -mthumb -mcpu=cortex-m4  -fno-threadsafe-statics -ffunction-sections -fno-rtti -flto -fno-use-cxa-atexit",
	ld_flags = "-O0 -g -nostdlib -mthumb -mcpu=cortex-m4 -fno-rtti -fno-exceptions -fdata-sections -ffunction-sections -flto -Wl,--gc-sections -fno-use-cxa-atexit",

	libs = ["m", "gcc"]
)

module("arch.stm32g4",
	defines=["STM32G4XX"],
	mdepends = ["arch.cortex-m4", "arch.stm32_common"]
)

module("arch.stm32g431rb",
	defines=["STM32G431xx"],
	mdepends=["arch.stm32g4"],
	ldscripts=["ldscripts/stm32g431.ld"]
)