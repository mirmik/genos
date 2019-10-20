import licant

#licant.execute("gpio/gpio.g.py")

licant.module("genos.drivers.common",
	sources = ["serial/uartring.cpp"]
)

licant.module("genos.drivers.avr.gpio", sources = [ "gpio/avr_gpio.c" ], 
	local_headers = [
		("__local__/arch_gpio.h", "gpio/avr_gpio.h")
	]
)
licant.module("genos.drivers.avr.spi", sources = ["spi/avr_spi.cpp"])
licant.module("genos.drivers.avr.usart", sources = [ "serial/avr_usart.cpp" ])
licant.module("genos.drivers.avr.timer", sources = [ "timer/avr_timer.cpp" ])
licant.module("genos.drivers.avr",
	mdepends = [
		"genos.drivers.common",
		#"genos.drivers.avr.spi", 
		"genos.drivers.avr.usart",
		"genos.drivers.avr.timer",
		"genos.drivers.avr.gpio",
	],
)

licant.module("genos.drivers.stm32_gpio", sources = ["gpio/stm32_gpio.c"], local_headers = [("__local__/arch_gpio.h", "gpio/stm32_gpio.h")])
licant.module("genos.drivers.stm32_usart", sources = ["serial/stm32_usart.cpp"])
licant.module("genos.drivers.stm32_spi", sources = ["spi/stm32-spi.cpp"])
licant.module("genos.drivers.stm32", 
	mdepends = [
		"genos.drivers.common",
		"genos.drivers.stm32_gpio",
		"genos.drivers.stm32_usart",
		"genos.drivers.stm32_spi",
	]
)

licant.module("genos.drivers.stm32l4_usart", sources = ["serial/stm32l4_usart.cpp"])
licant.module("genos.drivers.stm32l4", 
	mdepends = [
		"genos.drivers.common",
		"genos.drivers.stm32_spi",
		"genos.drivers.stm32l4_usart",
	]
)

licant.module("genos.drivers.atmel_dataflash", sources = ["bdev/atmel_dataflash.cpp"])


licant.module("genos.drivers.crow.uartgate",
	sources = ["crow/uartgate.cpp"]
)

licant.module("genos.drivers.bdev.atmel_dataflash",
	sources = ["bdev/atmel_dataflash.cpp"]
)