#!/usr/bin/env python3

import sys
import licant
from licant.modules import submodule, module
from licant.libs import include

licant.include("igris")
licant.include("zillot")
licant.execute("../genos.g.py")

licant.cxx_application("runtests",
                       sources=["*.cpp"],

                       include_paths=["."],
                       mdepends=[
                           "genos",
                           "zillot",
                           "zillot.mock",
                           "igris.include",
                           "igris.systime",
                       ],
                       libs=["nos", "igris"],

                       cxx_flags="-flto -ffunction-sections -fexceptions -fdata-sections -Wl,--gc-sections -g -fno-rtti",
                       cc_flags="-flto -ffunction-sections -fdata-sections -Wl,--gc-sections -g",
                       ld_flags="-flto -ffunction-sections -fdata-sections -Wl,--gc-sections -g -fno-rtti",
                       )

licant.ex("runtests")
