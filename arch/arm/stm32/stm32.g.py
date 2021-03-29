import licant.modules

module("arch.stm32_common.sources", 	
	sources= [
		"src/stm32_start.c",
		"src/stm32_vectors.S",
		"src/stm32_arch.c",
		"src/stm32_adc.c",
		"src/stm32_rcc.c",
		"src/stm32_gpio.c",
		"src/stm32_spi.c",
	#	"src/stm32_i2c.c",
		"src/stm32_usart.c",
		"src/stm32_diag.c",
		"src/stm32_systick.c",
		"src/stm32_clockbus.c",
		"src/stm32_pll.c",
	],
)

module("arch.stm32_common.sources_l4", 	
	sources= [
		"src/stm32_start.c",
		"src/stm32_vectors.S",
		"src/stm32_arch.c",
	#	"src/stm32_adc.c",
		"src/stm32_rcc.c",
		"src/stm32_gpio.c",
		"src/stm32_spi.c",
		"src/stm32_usart.c",
		"src/stm32_diag.c",
		"src/stm32_systick.c",
		"src/stm32_clockbus.c",
		#"src/stm32_pll.c",
	],
)

module("arch.stm32_common",
	include_paths = ["include", "st-device"],

	ldscripts= ["ldscripts/stm32_common.ld"],

	cc_flags = "-Os -g -Wl,--gc-sections -nostdlib -nostdinc -fdata-sections -ffunction-sections -Wl,--gc-sections -flto -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -D__FPU_USED=1",
	cxx_flags = "-Os -g -Wl,--gc-sections -nostdlib -nostdinc -fdata-sections -ffunction-sections -Wl,--gc-sections -flto -fno-rtti -fno-exceptions -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fno-threadsafe-statics  -D__FPU_USED=1 -ffunction-sections -fno-rtti -flto -fno-use-cxa-atexit",
	ld_flags = "-Os -g -nostdlib -mthumb -mcpu=cortex-m4 -fno-rtti -fno-exceptions -fdata-sections -ffunction-sections -flto -Wl,--gc-sections -fno-use-cxa-atexit -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16",

	libs = ["m", "gcc"]
)

module("arch.stm32_usb",
	sources = ["src/stm32_usb.c"])

module("arch.stm32g4",
	defines=["STM32G4XX", "CHIP_STM32G4XX"],
	sources=["src/stm32g4/stm32g4_sysinit.c"],
	mdepends = ["arch.cortex-m4", "arch.stm32_common", "arch.stm32_common.sources"]
)

module("arch.stm32f4",
	defines=["STM32F4XX", "CHIP_STM32F4XX"],
	sources=["src/stm32f4/stm32f4_sysinit.c"],
	mdepends = ["arch.cortex-m4", "arch.stm32_common", "arch.stm32_common.sources"]
)

module("arch.stm32l4",
	defines=["STM32L4XX", "CHIP_STM32L4XX"],
	sources=["src/stm32l4/stm32l4_sysinit.c"],
	mdepends = ["arch.cortex-m4", "arch.stm32_common", "arch.stm32_common.sources_l4"]
)

module("arch.stm32f446re",
	defines=["STM32F446xx"],
	mdepends=["arch.stm32f4"],
	ldscripts=["ldscripts/stm32f446re.ld"]
)

module("arch.stm32f401re", 
	defines = ["CHIP_STM32F401", "STM32F401xE"],
	mdepends = [ "arch.stm32f4" ],
	ldscripts = "ldscripts/stm32f401re.ld",
)

module("arch.stm32f407vg", 
	defines = ["CHIP_STM32F407", "STM32F407xx"],
	mdepends = [
		"arch.stm32f4"
	],
	ldscripts = "ldscripts/stm32f407vg.ld",
)

module("arch.stm32g431rb",
	defines=["STM32G431xx"],
	mdepends=["arch.stm32g4"],
	ldscripts=["ldscripts/stm32g431.ld"]
)

module("arch.stm32g474re",
	defines=["STM32G474xx"],
	mdepends=["arch.stm32g4"],
	ldscripts=["ldscripts/stm32g474re.ld"]
)


module("genos.hal", impl = "stm32l432", 
	defines = ["CHIP_STM32L432", "CHIP_STM32L432XX", "STM32L432xx"],
	mdepends=["arch.stm32l4", "arch.stm32_usb"],
	ldscripts = "ldscripts/stm32l432.ld",
)

module("arch.stm32f746ng",
	defines=["STM32F746xx"],
	mdepends=["arch.stm32f7"],
	ldscripts=["ldscripts/stm32f746ng.ld"]
)