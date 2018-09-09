import licant

licant.execute("gpio/gpio.g.py")

licant.module("genos.chardev", sources = [ "char/char_device.c" ] )