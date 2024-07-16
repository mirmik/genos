import licant

licant.module("zillot.arduino.avr-systime",
	sources = [
		"arduino-avr-systime.c"
	],
	mdepends=[]
)

licant.module("zillot.arduino-compat", 
	include_paths=["."], 
	defines=["ARDUINO=100"])

licant.module("zillot.arduino.mega",
	mdepends = [
		"zillot.arduino-compat",
		"zillot.arduino.avr-systime",
	],
	defines = [
		"BOARD_ARDUINO_MEGA=1",
		"F_CPU=16000000"
	]
)

licant.module("zillot.arduino.uno",
	mdepends = [
		"zillot.arduino-compat",
		"zillot.arduino.avr-systime",
	],
	defines = [
		"BOARD_ARDUINO_UNO=1",
		"F_CPU=16000000"
	]
)
