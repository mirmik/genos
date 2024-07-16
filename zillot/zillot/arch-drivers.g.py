import licant

licant.module("zillot.drivers.avr",
	mdepends = [
		"zillot.drivers.serial.avr",
		"zillot.drivers.i2c.avr",
	]
)