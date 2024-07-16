#!/usr/bin/env python3

import licant
import os

licant.include("igris")
licant.include("nos")
licant.include("onboardtest")
licant.include("zillot", "../../zillot.g.py")

licant.cxx_application("stm32-firmware.elf",
                       builddir="stm32build",
                       toolchain=licant.gcc_toolchain("arm-none-eabi-"),
                       sources=["*.cpp"],
                       include_paths=["."],
                       mdepends=[
                           "igris.libc",
                           "igris.std",
                           "igris.posix",
                           "igris.include",
                           "igris.cxx_support",
                           "igris.printf_impl",
                           ("igris.dprint", "zillot.diag"),
                           "igris.util",
                           "igris.utilxx",
                           "igris.lin_malloc",
                           "zillot.stm32.irqtable",
                           "zillot.irqtable",
                           "zillot",
                           ("igris.syslock", "irqs"),
                           ("igris.systime", "jiffies"),
                           "newlib-stub",
                           "zillot.stm32f446re",
                           "stm32.irqtable_ldscript.stm32f446re",
                           "nos.check",
                           "nos.include",
                           "nos.print",
                           "nos.fprint",
                           ("nos.current_ostream", "nullptr"),
                           "onboardtest"
                       ],
                       cxx_flags="-g -nostdlib -nostdinc -nodefaultlibs -ffunction-sections -fdata-sections -mthumb -mcpu=cortex-m4 -fno-exceptions -fno-rtti -fno-threadsafe-statics -mfloat-abi=hard -mfpu=fpv4-sp-d16  -D__FPU_USED=1",
                       cc_flags="-g -nostdlib -nostdinc -nodefaultlibs -ffunction-sections -fdata-sections -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16  -D__FPU_USED=1",
                       ld_flags="-g -nostdlib -nostdinc -nodefaultlibs -ffunction-sections -fdata-sections -Wl,--gc-sections -fmax-errors=1 -mthumb -mcpu=cortex-m4 -fno-exceptions -fno-rtti  -mfloat-abi=hard -mfpu=fpv4-sp-d16  -D__FPU_USED=1",
                       )


@licant.routine(deps=["stm32-firmware.elf"])
def flash():
    os.system(
        "arm-none-eabi-objcopy -O binary stm32-firmware.elf stm32-firmware.bin")
    os.system("st-flash write stm32-firmware.bin 0x8000000")


@licant.routine(deps=["stm32-firmware.elf"])
def install():
    os.system(
        "arm-none-eabi-objcopy -O binary stm32-firmware.elf stm32-firmware.bin")
    os.system("st-flash write stm32-firmware.bin 0x8000000")


@licant.routine
def remote_install(ip):
    os.system(
        "arm-none-eabi-objcopy -O binary stm32-firmware.elf stm32-firmware.bin")
    os.system(f"scp stm32-firmware.bin rfmeas@{ip}:~/stm32-firmware.bin")
    os.system(
        f"ssh rfmeas@{ip} 'st-flash --reset write stm32-firmware.bin 0x8000000'")


@licant.routine
def remote_stutil(ip):
    os.system(f"ssh mirmik@{ip} 'st-util'")


@licant.routine
def remote_test_start(ip):
    os.system(f"ssh mirmik@{ip} 'onboardtest-listener /dev/ttyACM0'")


@licant.routine
def remote_test(ip):
    print("Uploading...")
    os.system(
        "arm-none-eabi-objcopy -O binary stm32-firmware.elf stm32-firmware.bin")
    os.system(f"scp stm32-firmware.bin mirmik@{ip}:~/stm32-firmware.bin")
    os.system(
        f"ssh mirmik@{ip} 'st-flash --reset write stm32-firmware.bin 0x8000000'")

    print()
    print("Start test:")
    os.system(f"ssh mirmik@{ip} 'onboardtest-listener /dev/ttyACM0'")


@licant.routine
def remote_gdb(ip):
    os.system(
        f"gdb-multiarch stm32-firmware.elf -ex 'target remote {ip}:4242'")


if __name__ == "__main__":
    licant.ex("stm32-firmware.elf")
