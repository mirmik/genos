import licant

licant.module("zillot.avr",
	sources=[
		#"start.S",
		#"vectors.S",
		#"irqcall.S",
		"switch_context.S",
		"context.c",
		"end_of_programm.c",
		"arch.c",
		"avr_gpio.c",
		"avr_timer.c",
		"debug_blink.S",
		"avr_i2c_device.c",
		"diag.c",
	],

	include_paths = ["internal/"],
	mdepends=["zillot.avr.drivers"]
)

licant.module("zillot.avr.irqtable",
	sources=[
		"irqcall.S",
	]
)

licant.module("zillot.avr.drivers",
	sources=["usart.cpp"]
)

licant.module("zillot.avr.atmega328p",
	mdepends = ["zillot.avr"],
	#ldscripts = ["ldscripts/atmega328p.ld"],
	defines = ["CHIP_ATMEGA328P"],
	cxx_flags = "-mmcu=atmega328p",
	cc_flags = "-mmcu=atmega328p",
	ld_flags = "-mmcu=atmega328p",
)


licant.module("zillot.avr.atmega2560",
	mdepends = ["zillot.avr"],
	#ldscripts = ["ldscripts/atmega2560.ld"],
	defines = ["CHIP_ATMEGA2560"],
	cxx_flags = "-mmcu=atmega2560",
	cc_flags = "-mmcu=atmega2560",
	ld_flags = "-mmcu=atmega2560",
)

