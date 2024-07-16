#!/usr/bin/env python3

import licant
import os

licant.include("igris")
licant.include("nos")
licant.include("crow")
licant.include("rabbit")
licant.include("genos")
licant.include("ralgo")
licant.include("zillot")

arm_binutils = licant.cxx_make.make_gcc_binutils("arm-none-eabi")

licant.module("firmware-common",
              sources=[
                  "src/*.cpp",
                  "src/*.c",
              ],
              include_paths=[".", "src"],
              mdepends=[
                  "genos.shellutil",
                  "igris.libc",
                  "genos.posix",
                  "igris.std",
                  "igris.lin_malloc",
                  "newlib-stub",
                  "igris.cxx_support",
                  "igris.include",
                  "genos.include",
                  "nos.util",
                  "nos.print",
                  "nos.fprint",
                  "nos.include",
                  "nos.shell",
                  "nos.log",
                  ("nos.current_ostream", "nullptr"),
                  ("igris.systime", "jiffies"),
                  "igris.util",
                  "igris.utilxx",
                  "igris.bug",
                  ("igris.dprint", "zillot.diag"),
                  "igris.printf_impl",
                  ("igris.syslock", "irqs"),
                  "igris.protocols.gstuff",
                  "genos",
                  "crow.minimal",
                  ("crow.allocator", "malloc"),
                  ("crow.diagnostic", "debug"),
                  ("ralgo.disctime", "__none__"),
                  ("ralgo.log", "__none__"),
                  "ralgo",
                  "ralgo.include",
                  "zillot.stm32.vectors",
                  "zillot.include",
                  "zillot",
                  "zillot.irqtable",
              ],

              defines=["SCHEDEE_DEBUG_STRUCT=1"],
              )

licant.cxx_application("firmware-mini.elf",
                       builddir="build-mini",
                       toolchain=arm_binutils,
                       sources=[
                           "src/specific/mini/*.cpp",
                           "src/trigger/*.cpp",
                       ],
                       include_paths=["src/specific/mini"],
                       mdepends=[
                           "firmware-common",
                           "zillot.stm32f407vg",
                           "stm32.irqtable_ldscript.stm32f407vg",
                       ],

                       defines=[
                           "WITHOUT_TRIGGER=0",
                           "HELIX_BOARD_NAME=mini"
                       ],
                       cxx_flags="-nostdinc -nostdlib -nodefaultlibs -mthumb -mcpu=cortex-m4 -fno-exceptions -fno-rtti -fno-threadsafe-statics -mfloat-abi=hard -mfpu=fpv4-sp-d16  -D__FPU_USED=1",
                       cc_flags="-nostdinc -nostdlib -nodefaultlibs -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16  -D__FPU_USED=1",
                       ld_flags="-nostdinc -nostdlib -nodefaultlibs -mthumb -mcpu=cortex-m4 -fno-exceptions -fno-rtti  -mfloat-abi=hard -mfpu=fpv4-sp-d16  -D__FPU_USED=1",
                       )

licant.cxx_application("firmware-multiax.elf",
                       builddir="build-multiax",
                       toolchain=arm_binutils,
                       sources=[
                           "src/specific/multiax/*.cpp",
                           # "src/trigger/*.cpp",
                       ],
                       include_paths=["src/specific/multiax"],
                       mdepends=[
                           "firmware-common",
                           "zillot.stm32h723zg",
                           "stm32.irqtable_ldscript.stm32h723zg",
                       ],

                       defines=[
                           "WITHOUT_TRIGGER=1",
                           "HELIX_BOARD_NAME=multiax"
                       ],
                       cxx_flags="-O0 -nostdinc -nostdlib -nodefaultlibs -mthumb -mcpu=cortex-m7 -fno-exceptions -fno-rtti -fno-threadsafe-statics -mfloat-abi=hard -mfpu=fpv4-sp-d16",
                       cc_flags="-O0 -nostdinc -nostdlib -nodefaultlibs -mthumb -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv4-sp-d16",
                       ld_flags="-O0 -flto -nostdinc -nostdlib -nodefaultlibs -mthumb -mcpu=cortex-m7 -fno-exceptions -fno-rtti  -mfloat-abi=hard -mfpu=fpv4-sp-d16" +
                       " ",
                       )


@licant.routine(deps=["firmware-mini.elf"])
def install(buildname):
    os.system(
        f"arm-none-eabi-objcopy -O binary firmware-{buildname}.elf firmware-{buildname}.bin")
    os.system(f"st-flash --reset write firmware-{buildname}.bin 0x8000000")


@licant.routine
def terminal():
    os.system("gtkterm -p /dev/ttyACM0 -s 115200 --parity none")


@licant.routine
def remote_install(ip, name):
    os.system(
        f"arm-none-eabi-objcopy -O binary firmware-{name}.elf firmware-{name}.bin")
    os.system(f"scp firmware-{name}.bin rfmeas@{ip}:~/firmware-{name}.bin")
    os.system(
        f"ssh rfmeas@{ip} 'st-flash --reset write firmware-{name}.bin 0x8000000'")

@licant.routine
def remote_erase(ip):
    os.system(
        f"ssh rfmeas@{ip} 'st-flash --connect-under-reset erase'")


@licant.routine
def remote_stutil(ip):
    os.system(f"ssh rfmeas@{ip} 'st-util'")


@licant.routine
def remote_gdb(ip, name):
    os.system(
        f"gdb-multiarch firmware-{name}.elf -ex 'target remote {ip}:4242'")


licant.fileset("mini", ["firmware-mini.elf"])
licant.fileset("multiax", ["firmware-multiax.elf"])

licant.fileset("all", ["mini", "multiax"])
licant.ex("all")
