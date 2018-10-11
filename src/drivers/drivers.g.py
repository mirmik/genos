import licant

licant.execute("gpio/gpio.g.py")

licant.module("genos.chardev", sources = [ "char/char_device.c" ] )


licant.module("genos.drivers.spi.avr", sources = ["spi/avr_spi.c"])
licant.module("genos.drivers.usart.avr", sources = ["serial/avr_usart.c"])
licant.module("genos.drivers.avr",
	include_modules = [
		submodule("genos.drivers.spi.avr"), 
		submodule("genos.drivers.usart.avr"),
		#"genos.drivers.gpio.avr",
	],
)
