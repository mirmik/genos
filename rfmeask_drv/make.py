#!/usr/bin/env python3

import licant
import os

licant.include("igris")
licant.include("genos")
#licant.include("nos")
licant.include("zillot")

arm_binutils = licant.cxx_make.make_gcc_binutils("arm-none-eabi")

STM32_FLAGS = " -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -D__FPU_USED=1"

licant.cxx_application("firmware.elf",
                       toolchain=arm_binutils,

                       sources=[
                           "src/main.cpp",
                           "src/rs232.cpp",
                           "src/console.cpp",
                           "src/configure.cpp",
                           "src/board.cpp",
                           "src/axis.cpp",
                           "src/comparectr.cpp",
                           "src/functions.cpp",
                           "src/utility/numcmd.cpp",
                           "src/utility/contty2.cpp",
                           "src/utility/mshell.c",
                           "src/stm32_arch.c",
                           "src/distance_table.cpp",
                       ],

                       include_paths=["src"],

                       mdepends=[
                           "genos.include",
				"genos",
                           "igris.include",
                           "zillot.include",
                           "igris.stdlibs",
                           "igris.bug",
                           "igris.utilxx",
                           "igris.lin_malloc",
                           ("igris.syslock", "irqs"),
                           ("igris.dprint", "zillot.diag"),
                           ("igris.systime", "jiffies"),
                           "igris.protocols.numcmd",
                           "genos.schedee",
                           "igris.cxx_support",
                           "igris.printf_impl",
                           "newlib-stub",
                           "zillot.stm32f407vg",
                           "zillot.common",
                           "zillot.irqtable",
                           "zillot.stm32.vectors",
				"zillot.posix-stub",
                           "stm32.irqtable_ldscript.stm32f407vg"
                       ],

                       defines=["SCHEDEE_DEBUG_STRUCT=1"],
                       cc_flags="-O0 -Werror=all -nostdinc" + STM32_FLAGS,
                       cxx_flags="-O0 -Werror=all -fno-exceptions -fno-rtti  -nostdinc -fno-use-cxa-atexit" + STM32_FLAGS,
                       ld_flags="-O0 -Wl,--gc-sections -fno-exceptions -fno-rtti  -nostdlib -fdata-sections -ffunction-sections -flto -Wl,--gc-sections -fno-use-cxa-atexit" + STM32_FLAGS,
                       libs=["m", "gcc"]
                       )


@licant.routine(deps=["firmware.elf"])
def install():
    os.system("arm-none-eabi-objcopy -O binary firmware.elf firmware.bin")
    os.system("st-flash write firmware.bin 0x8000000")


@licant.routine
def terminal():
    os.system("sudo gtkterm -p /dev/ttyUSB0 -s 115200 --parity even")


@licant.routine
def remote_install(ip):
    os.system("arm-none-eabi-objcopy -O binary firmware.elf firmware.bin")
    os.system(f"scp firmware.bin rfmeas@{ip}:~/firmware.bin")
    os.system(
        f"ssh rfmeas@{ip} 'st-flash --reset write firmware.bin 0x8000000'")


@licant.routine
def remote_stutil(ip):
    os.system(f"ssh rfmeas@{ip} 'st-util'")


@licant.routine
def remote_gdb(ip):
    os.system(f"gdb-multiarch firmware.elf -ex 'target remote {ip}:4242'")


licant.ex("firmware.elf")
