import licant

licant.execute("gpio/gpio.g.py")

licant.module("genos.drivers.common",
	sources = ["serial/uartring.cpp"]
)

licant.module("genos.drivers.spi.avr", sources = ["spi/avr_spi.cpp"])
licant.module("genos.drivers.usart.avr", sources = [ "serial/avr_usart.cpp" ])
licant.module("genos.drivers.avr",
	include_modules = [
		submodule("genos.drivers.common"),
		submodule("genos.drivers.spi.avr"), 
		submodule("genos.drivers.usart.avr"),
		#"genos.drivers.gpio.avr",
	],
)

licant.module("genos.drivers.atmel_dataflash", sources = ["bdev/atmel_dataflash.cpp"])