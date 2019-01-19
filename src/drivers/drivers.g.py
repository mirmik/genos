import licant

licant.execute("gpio/gpio.g.py")

licant.module("genos.drivers.common",
	sources = ["serial/uartring.cpp"]
)

licant.module("genos.drivers.spi.avr", sources = ["spi/avr_spi.cpp"])
licant.module("genos.drivers.usart.avr", sources = [ "serial/avr_usart.cpp" ])
licant.module("genos.drivers.avr",
	mdepends = [
		"genos.drivers.common",
		"genos.drivers.spi.avr", 
		"genos.drivers.usart.avr",
		#"genos.drivers.gpio.avr",
	],
)

licant.module("genos.drivers.atmel_dataflash", sources = ["bdev/atmel_dataflash.cpp"])


licant.module("genos.drivers.crow.uartgate",
	sources = ["crow/uartgate.cpp"]
)

licant.module("genos.drivers.bdev.atmel_dataflash",
	sources = ["bdev/atmel_dataflash.cpp"]
)