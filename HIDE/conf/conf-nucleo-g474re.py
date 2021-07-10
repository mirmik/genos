import licant
import os

toolchain = licant.cxx_make.toolchain_gcc("arm-none-eabi-")

mdepends = [
	"board.nucleo-g474re",
	"drivers.stm32g4"
]

def install(file, *args):
	os.system("arm-none-eabi-objcopy -O binary {file} firmware.bin".format(file=file))
	os.system("st-flash erase")
	os.system("st-flash write firmware.bin 0x8000000")
	os.system("st-flash reset")

def terminal(path="/dev/ttyACM0"):
	os.system("gtkterm -p {} -s 115200 --parity none".format(path))