import licant
import os

toolchain = licant.cxx_make.toolchain_gcc("avr-")

mdepends = [
	("genos.board", "arduino_uno"),
	"genos.drivers.avr"
]

default_file = "/dev/ttyACM0"

def install(src="firmware", tgt=default_file, *args):
	cmd = "avrdude -P{tgt} -v -carduino -patmega328p -b115200 -D -Uflash:w:./{src} -u".format(src=src, tgt=tgt)
	print(cmd)
	os.system(cmd)

def terminal(path=default_file):
	os.system("sudo gtkterm -p {} -s 115200 --parity none".format(path))
