#!/usr/bin/env python3
import licant
import os

licant.include("igris")
licant.include("zillot", "../../zillot.g.py")

licant.cxx_application("arduino2560-firmware.elf",
                       builddir="arduino2560build",
                       toolchain=licant.gcc_toolchain("avr-"),
                       sources=["main.cpp"],
                       include_paths=["/usr/lib/avr/include/"],
                       mdepends=[
                           "igris.libc",
                           "igris.compat.std",
                           "igris.include",
                           "igris.util",
                           ("igris.systime", "jiffies"),
                           ("igris.syslock", "irqs"),
                           ("igris.dprint", "zillot.diag"),
                           "igris.cxx_support",
                           "zillot",
                           "zillot.posix-stub",
                           "zillot.avr.atmega2560",
                           "zillot.arduino.mega",
                       ],

                       cxx_flags="-ffunction-sections -fdata-sections",
                       cc_flags="-ffunction-sections -fdata-sections",
                       ld_flags="-nostdinc -Wl,--gc-sections",
                       libs=["gcc"],
                       defines=["WITHOUT_ATOF64"]
                       )

default_file = "/dev/ttyACM0"


@licant.routine(deps=["arduino2560-firmware.elf"])
def install(src="firmware.hex", tgt=default_file, *args):
    for cmd in [
            "avr-objcopy -O ihex -R .eeprom -R .fuse arduino2560-firmware.elf arduino2560-firmware.hex",
            f"avrdude -P{tgt} -v -cwiring -patmega2560 -b115200 -D -Uflash:w:./{src} -u"
    ]:
        print(cmd)
        os.system(cmd)


@licant.routine
def terminal(path=default_file):
    os.system("sudo gtkterm -p {} -s 115200 --parity none".format(path))


if __name__ == "__main__":
    licant.ex("arduino2560-firmware.elf")
