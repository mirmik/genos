import licant

licant.module("zillot.i2c-common",
	sources=["i2c_device.c"]
)

licant.module("zillot.drivers.i2c.avr",
	sources = ["avr_i2c_device.c"],
	mdepends = ["zillot.i2c-common"]
)