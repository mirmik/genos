import licant
import os

toolchain = licant.cxx_make.toolchain_gcc("arm-none-eabi-")

mdepends = [
	("genos.board", "rl_syncro_v2"),
]

@licant.routine(deps=["firmware"])
def install(file, *args):
	os.system("arm-none-eabi-objcopy -O binary firmware firmware.bin")
	os.system("st-flash write firmware.bin 0x8000000")

@licant.routine
def terminal():
	os.system("sudo gtkterm -p /dev/ttyUSB0 -s 115200 --parity none")
