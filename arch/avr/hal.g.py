from licant.modules import module, submodule

module("genos.hal.avr.common",
	sources = [
		"src/asm/arch.cpp",
		"src/asm/diag.c",
		"src/asm/systime.cpp",

		"src/drivers/gpio.c",
		"src/drivers/timer.c",
		#"src/drivers/usart.c",

		"src/asm/atmega/*"
	],
	include_paths = ["include"]
)

module("genos.hal", impl = "atmega2560",
	include_paths = ["atmega2560/include"],
	include_modules = [submodule("genos.hal.avr.common")],
	ldscripts = ["atmega2560/ldscripts/flash.ld"],

	defines = ["CHIP_ATMEGA2560"],
	cxx_flags = "-nostdlib -lgcc -lm -mmcu=atmega2560 -Wl,--gc-sections -fdata-sections -ffunction-sections -fno-rtti -mmcu=atmega2560",
	cc_flags = "-nostdlib -lgcc -lm -mmcu=atmega2560 -Wl,--gc-sections -fdata-sections -ffunction-sections -mmcu=atmega2560",
	ld_flags = "-nostdlib -lgcc -lm -mmcu=atmega2560 -Wl,--gc-sections -fdata-sections -ffunction-sections",
)

#Module("genos.arch.avr.head", {
#	sources = {
#		cc = {
#			"src/asm/atmega/vectors.S", 
#			"src/asm/atmega/irqcall.S", 
#			"src/asm/atmega/start.S",  
#			"src/asm/atmega/end_of_programm.c", 
#			"src/asm/context.c", 
#			"src/asm/switch_context.S",
#		}
#	}
#})
#
#Module("genos.arch.atmega2560", {
#	modules = {
#		{name = "genos.arch.avr.head"}
#	},
#
#	sources = {
#		cc = {
#			"src/drivers/gpio.c",
#			"src/asm/diag.c",
#			"src/asm/arch.c",
#			"src/drivers/timer_device.c",
#			"src/asm/systime.c"
#		}
#	}
#})
#
#Module("genos.include.arch.atmega2560", {
#	includePaths = "include atmega2560/include",
#	ldscripts = "atmega2560/ldscripts/flash.ld",
#	depends = "genos.arch.atmega2560"
#}) 
#
#Module("gxx.avr", {
#	sources = {
#		cxx = {"gxx/atomic.cpp"}
#	}
#}) 
#
#--[[regmodule {
#	name = "arch.avr2560",
#	sources = {
#		cc = {"src/asm/systime.c", "src/asm/arch.c", "src/asm/procdelay.c", "atmega2560/src/periph/devices.c"},
#	},
#	modules = {
#	--	{name = "avr_gpio"},
#		{name = "avr_periph_usart"},
#		{name = "avr_periph_tc"},
#	},
#}
#
#regmodule {
#	name = "avr_periph_usart",
#	sources = {
#		cc = {"src/dev/usart.c"},
#	},
#}
#
#regmodule {
#	name = "avr_periph_tc",
#	sources = {
#		cc = {"src/drivers/timer_device.c"},
#	},
#}
#
#regmodule {
#	name = "arch_diag.avr2560",
#	sources = {
#		cc = {"src/asm/diag.c"},
#	}
#}
#
#regmodule {
#	name = "subst.avr2560",
#	sources = {
#		cc = {"src/asm/context.c", "src/asm/switch_context.S"},
#	}
#}
#
#regmodule {
#	name = "avr_gpio",
#	sources = {
#		cc = {"src/drivers/gpio.c"},
#	},
#}]]#