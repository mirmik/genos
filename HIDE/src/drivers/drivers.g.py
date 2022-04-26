import licant

#licant.execute("gpio/gpio.g.py")

licant.module("genos.drivers.common",
	sources = [
		"serial/uartring.cpp",
		"display/display.cpp",
	]
)

#licant.module("genos.drivers.avr.spi", sources = ["spi/avr_spi.cpp"])
licant.module("drivers.avr.gpio", sources=["gpio/avr_gpio.cpp"])
licant.module("genos.drivers.avr.usart", sources = [ "serial/avr_usart.cpp" ])
licant.module("genos.drivers.avr.timer", sources = [ "timer/avr_timer.cpp" ])
licant.module("genos.drivers.avr",
	mdepends = [
		"genos.drivers.common",
		#"genos.drivers.avr.spi", 
		"genos.drivers.avr.usart",
#		"genos.drivers.avr.timer",
	],
)

licant.module("genos.drivers.stm32_gpio", sources = ["gpio/stm32_gpio.c"], local_headers = [("__local__/arch_gpio.h", "gpio/stm32_gpio.h")])
licant.module("genos.drivers.stm32_usart", sources = ["serial/stm32_usart.cpp"])
licant.module("genos.drivers.stm32_spi", sources = ["spi/stm32-spi.cpp"])
licant.module("genos.drivers.stm32_i2c", sources = ["i2c/stm32_i2c.cpp"])
licant.module("genos.drivers.stm32", 
	mdepends = [
		"genos.drivers.common",
		"genos.drivers.stm32_gpio",
		"genos.drivers.stm32_usart",
		"genos.drivers.stm32_spi",
#		"genos.drivers.stm32_i2c",
	]
)

#licant.module("genos.drivers.stm32l4_usart", sources = ["serial/stm32l4_usart.cpp"])
licant.module("genos.drivers.stm32l4", 
	mdepends = [
		"genos.drivers.common",
		"genos.drivers.stm32_spi",
		"genos.drivers.stm32_gpio",
		"genos.drivers.stm32_usart",
	]
)

licant.module("genos.drivers.atmel_dataflash", sources = ["bdev/atmel_dataflash.cpp"])


licant.module("genos.drivers.crow.uartgate",
	sources = ["crow/uartgate.cpp"]
)

licant.module("genos.drivers.bdev.atmel_dataflash",
	sources = ["bdev/atmel_dataflash.cpp"]
)


licant.module("drivers.stm32g4",
	mdepends=[
		"genos.drivers.common",
		"genos.drivers.stm32_gpio",
		"genos.drivers.stm32_usart",
		"genos.drivers.stm32_spi",
	]
)

licant.module("genos.drivers.stm32f7", 
	sources=[
		"gpio/stm32_gpio.c",
		"serial/stm32_usart.cpp",
	]
)


licant.module("arduino.sd_driver", 
	sources=[
		"sd/SdFile.cpp",
		"sd/SdVolume.cpp",
		"sd/SD.cpp",
		"sd/File.cpp",
	]
)

licant.module("adafruit_ssd1306", 
	sources=[
		"display/Adafruit_SSD1306.cpp",
	]
)