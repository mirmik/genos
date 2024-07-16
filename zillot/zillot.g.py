#!/usr/bin/env python3

import licant

licant.execute_recursive("zillot", ".g.py")

licant.module("zillot.diag", "stub",
              sources=["zillot/diag_stub.c"]
              )

licant.module("zillot.diag", "impl",
              sources=["zillot/diag_impl.c"],
              default=True
              )

licant.module("igris.dprint", "zillot.diag",
              mdepends=[
                  "zillot.diag",
                  "igris.dprint.common"
              ],
              sources=["zillot/dprint_diag.c"]
              )

licant.module("zillot.include",
              include_paths=["."]
              )

licant.module("zillot.arduino.avr.systime",
              sources=["zillot/arduino/avr-systick.c"]
              )

licant.module("zillot.common",
              sources=[
                  "zillot/common/*.cpp",
                  "zillot/i2c/i2c_device.c"
              ]
              )

licant.module("zillot.irqtable",
              sources=["zillot/irqtable/*.cpp"],
              defines=["ZILLOT_WITH_IRQTABLE"]
              )

licant.module("zillot",
              mdepends=[
                  "zillot.include",
                  "zillot.common",
                  "igris.semaphore",
              ]
              )

licant.module("zillot.posix-stub", 
                include_paths=["zillot/posix-stub"],
                sources=["zillot/posix-stub/*.cpp", "zillot/posix-stub/*.c"]
)