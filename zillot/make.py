#!/usr/bin/env python3
import licant
import os

licant.execute("./tests/arduino2560-test/make.py")
licant.execute("./tests/stm32-test/make.py")
licant.execute("./tests/stm32h7-test/make.py")
licant.execute("./tests/common/make.py")

licant.fileset(
    "all", ["runtests", 
            "arduino2560-firmware.elf", 
            "stm32-firmware.elf",
            "stm32h7-firmware.elf"])


@licant.routine(deps=["all"])
def stm32_install_test():
    os.system(
        "arm-none-eabi-objcopy -O binary stm32-firmware.elf stm32-firmware.bin")
    os.system("st-flash write stm32-firmware.bin 0x8000000")

@licant.routine(deps=["all"])
def stm32h7_install_test():
    os.system(
        "arm-none-eabi-objcopy -O binary stm32h7-firmware.elf stm32h7-firmware.bin")
    os.system("st-flash write stm32h7-firmware.bin 0x8000000")


@licant.routine(deps=["all"])
def install():
    return


licant.ex("all")
