import licant

#licant.execute("gpio/gpio.g.py")

licant.module("genos.drivers.common",
	sources = ["serial/uartring.cpp", "serial/uartring2.c"]
)

licant.module("genos.drivers.spi.avr", sources = ["spi/avr_spi.cpp"])
licant.module("genos.drivers.usart.avr", sources = [ "serial/avr_usart.cpp" ])
licant.module("genos.drivers.timer.avr", sources = [ "timer/avr_timer.cpp" ])
licant.module("genos.drivers.avr",
	mdepends = [
		"genos.drivers.common",
		"genos.drivers.spi.avr", 
		"genos.drivers.usart.avr",
		"genos.drivers.timer.avr",
		"genos.drivers.gpio.avr",
	],
)

licant.module("genos.drivers.stm32_gpio", sources = ["gpio/stm32_gpio.c"], local_headers = [("__local__/arch_gpio.h", "gpio/stm32_gpio.h")])
licant.module("genos.drivers.stm32_usart", sources = ["serial/stm32_usart.c"])
licant.module("genos.drivers.stm32", 
	mdepends = [
		"genos.drivers.common",
		"genos.drivers.stm32_gpio",
		"genos.drivers.stm32_usart"
	]
)

licant.module("genos.drivers.atmel_dataflash", sources = ["bdev/atmel_dataflash.cpp"])


licant.module("genos.drivers.crow.uartgate",
	sources = ["crow/uartgate.cpp"]
)

licant.module("genos.drivers.bdev.atmel_dataflash",
	sources = ["bdev/atmel_dataflash.cpp"]
)