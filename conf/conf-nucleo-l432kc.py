import licant
import os

toolchain = licant.cxx_make.toolchain_gcc("arm-none-eabi-")

mdepends = [
	("genos.board", "nucleo-l432kc"),
	"genos.drivers.stm32l4"
]

def install(file, *args):
	os.system("arm-none-eabi-objcopy -O binary {file} firmware.bin".format(file=file))
	os.system("st-flash erase")
	os.system("st-flash write firmware.bin 0x8000000")

def terminal(path="/dev/ttyACM0"):
	os.system("gtkterm -p {} -s 115200 --parity none".format(path))