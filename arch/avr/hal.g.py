from licant.modules import module, submodule

module("avr.diag", sources=["src/diag.c"])

module("genos.hal.avr.common",
	srcdir = "src",

	sources = [
		"start.S",
		"vectors.S",
		"irqcall.S",

		"switch_context.S",
		"context.c",

		"end_of_programm.c",

		"arch.c",

		"avr_gpio.c",
		"avr_timer.c",
		"debug_blink.S"
	],
	mdepends=["avr.diag",
		"genos.irqtable",
		"genos.systime"
	],

	include_paths = ["include"]
)

module("cpu.avr.common",
	include_paths = ["include"],
	sources=["src/avr_gpio.c"]
)

module("cpu.avr.atmega2560",
	mdepends = ["cpu.avr.common"],
	defines = ["CHIP_ATMEGA2560"],
	cxx_flags = "-mmcu=atmega2560",
	cc_flags = "-mmcu=atmega2560",
	ld_flags = "-mmcu=atmega2560",
)

module("cpu.avr.atmega168",
	mdepends = ["cpu.avr.common"],
	defines = ["CHIP_ATMEGA168"],
	cxx_flags = "-mmcu=atmega168",
	cc_flags = "-mmcu=atmega168",
	ld_flags = "-mmcu=atmega168",
)

module("genos.hal", impl = "atmega2560",
	mdepends = ["genos.hal.avr.common"],
	ldscripts = ["ldscripts/atmega2560.ld"],

	defines = ["CHIP_ATMEGA2560"],
	cxx_flags = "-nostdlib -mmcu=atmega2560 -Wl,--gc-sections -fdata-sections -fpermissive -DNDEBUG -fno-threadsafe-statics -ffunction-sections -fno-rtti -flto",
	cc_flags = "-nostdlib -mmcu=atmega2560 -Wl,--gc-sections -fdata-sections -DNDEBUG  -ffunction-sections -flto",
	ld_flags = "-nostdlib -mmcu=atmega2560 -fdata-sections -ffunction-sections -flto -Wl,--gc-sections ",
	libs = ["m", "gcc"],
)

module("genos.hal", impl = "atmega328p",
	mdepends = ["genos.hal.avr.common"],
	ldscripts = ["ldscripts/atmega328p.ld"],

	defines = ["CHIP_ATMEGA328P"],
	cxx_flags = "-nostdlib -mmcu=atmega328p -Wl,--gc-sections -fdata-sections -fpermissive -DNDEBUG -fno-threadsafe-statics -ffunction-sections -fno-rtti -flto",
	cc_flags = "-nostdlib -mmcu=atmega328p -Wl,--gc-sections -fdata-sections -DNDEBUG  -ffunction-sections -flto",
	ld_flags = "-nostdlib -mmcu=atmega328p -fdata-sections -ffunction-sections -flto -Wl,--gc-sections ",
	libs = ["m", "gcc"],
)

module("genos.hal", impl = "atmega328",
	mdepends = ["genos.hal.avr.common"],
	ldscripts = ["ldscripts/atmega328p.ld"],

	defines = ["CHIP_ATMEGA328"],
	cxx_flags = "-nostdlib -mmcu=atmega328p -Wl,--gc-sections -fdata-sections -fpermissive -DNDEBUG -fno-threadsafe-statics -ffunction-sections -fno-rtti -flto",
	cc_flags = "-nostdlib -mmcu=atmega328p -Wl,--gc-sections -fdata-sections -DNDEBUG  -ffunction-sections -flto",
	ld_flags = "-nostdlib -mmcu=atmega328p -fdata-sections -ffunction-sections -flto -Wl,--gc-sections ",
	libs = ["m", "gcc"],
)

module("genos.hal", impl = "atmega168",
	mdepends = ["genos.hal.avr.common"],
	ldscripts = ["ldscripts/atmega168.ld"],

	defines = ["CHIP_ATMEGA168"],
	cxx_flags = "-nostdlib -Os -mmcu=atmega168 -Wl,--gc-sections -fdata-sections -fpermissive -DNDEBUG -fno-threadsafe-statics -ffunction-sections -fno-rtti -flto",
	cc_flags = "-nostdlib -Os -mmcu=atmega168 -Wl,--gc-sections -fdata-sections -DNDEBUG  -ffunction-sections -flto",
	ld_flags = "-nostdlib -Os -mmcu=atmega168 -fdata-sections -ffunction-sections -flto -Wl,--gc-sections ",
	libs = ["m", "gcc"],
)
