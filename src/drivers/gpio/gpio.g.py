import licant

licant.module("genos.drivers.gpio.avr", 
	sources = [ "avr_gpio.c" ], 
	local_headers = [
		("__local__/arch_gpio.h", "avr_gpio.h")
	]
)