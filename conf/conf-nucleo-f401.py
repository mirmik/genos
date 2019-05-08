import licant
import os

binutils = licant.cxx_make.make_gcc_binutils("arm-none-eabi")

mdepends = [
	("genos.board", "nucleo-f401re"),
	"genos.drivers.stm32"
]

def install(file, *args):
	os.system("arm-none-eabi-objcopy -O binary {file} firmware.bin".format(file=file))
	os.system("st-flash write firmware.bin 0x8000000")

def terminal(path="/dev/ttyACM0"):
	os.system("sudo gtkterm -p {} -s 115200 --parity none".format(path))