import licant

licant.module("drivers.avr_gpio", 
	sources = [ "avr_gpio.c" ], 
	local_headers = [
		("__local__/arch_gpio.h", "avr_gpio.h")
	]
)