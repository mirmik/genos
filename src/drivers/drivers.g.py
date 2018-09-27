import licant

licant.execute("gpio/gpio.g.py")

licant.module("genos.chardev", sources = [ "char/char_device.c" ] )


licant.module("genos.drivers.spi.avr", sources = ["spi/avr_spi.c"])