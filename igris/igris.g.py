import licant
import sys

licant.execute("igris/platform/host/host.g.py")

licant.module("igris.portable_base",
              mdepends=[
                  "igris.include",
                  "igris.util",
                  "igris.bug",
                  ("igris.dprint", "user"),
              ]
              )

MODULES = [
    "igris.include",
    "igris.utilxx",
    "igris.bug",
    "igris.dprint",
    "igris.systime",
    "igris.semaphore",
    "igris.syslock",
    "igris.ctrobj",
    "igris.series",
    "igris.protocols.gstuff",
    "igris.crypt.aes"
]

licant.module("igris",
              sources=[
                  "igris/datastruct/stimer.c",
                  "igris/deprecated/path.cpp",
              ],
              mdepends=MODULES)

licant.module("igris.sclonner", sources=["igris/sclonner.cpp"])

licant.module("igris.executor", sources=["igris/shell/executor.cpp", ])

licant.module("igris.include", include_paths=["."])

######################STDLIBS#COMPAT############################

licant.execute("compat/libc/libc.g.py")
licant.execute("compat/std/std.g.py")
licant.execute("compat/stubarch/stubarch.g.py")
licant.execute("compat/newlib-stub/newlib-stub.g.py")
licant.execute("compat/mem/lin_malloc.g.py")

licant.module("igris.cxx_support", sources=[
    "compat/cxx/__cxa_pure_virtual.c",
    "compat/cxx/__dso_handle.c"
])

licant.module("igris.series",
              sources=[
                  "igris/series/series.cpp",
                  "igris/series/iterator.cpp",
                  "igris/series/block.cpp",
                  "igris/series/field_annotation.cpp",
              ]
              )

licant.module("igris.stdlibs",
              mdepends=["igris.libc", "igris.std"])

################################################################
######################DPRINT####################################

licant.module("igris.dprint.common", srcdir="igris/dprint",
              sources=["dprint_func_impl.c"],
              )

licant.module("igris.dprint", "user", srcdir="igris/dprint",
              sources=["dprint_manually.c"],
              mdepends=["igris.dprint.common"],
              )

licant.module("igris.dprint", "stdout", srcdir="igris/dprint",
              sources=["dprint_stdout.c"],
              mdepends=["igris.dprint.common"],
              default=True
              )

licant.module("igris.dprint", "stub", srcdir="igris/dprint",
              mdepends=[
                  "igris.dprint.common"
              ],
              sources=["dprint_stub.c"]
              )

#################################################################
########################BUG######################################

licant.module("igris.bug", impl="abort", sources=[
              "igris/util/bug_abort.c"], default=True)

#################################################################
########################PROTOCOLS################################

licant.module("igris.protocols.gstuff",
              sources=[
                  "igris/protocols/gstuff.cpp",
                  "igris/protocols/gstuff_v1/gstuff.c",
                  "igris/protocols/gstuff_v1/autorecv.c"
              ]
              )

licant.module("igris.protocols.numcmd",
              sources=[
                  "igris/protocols/numcmd/numcmd.c"
              ]
              )

#################################################################
#########################UTILS###################################

licant.module("igris.util", sources=[
    "igris/util/numconvert.c",
    "igris/util/hexascii.c",
    "igris/util/dstring.c",
    "igris/util/stub.c",
    "igris/util/crc.c",
    "igris/string/memmem.c",
    "igris/string/strdup.c",
    "igris/string/replace_substrings.c",
    "igris/shell/mshell.c",
    "igris/shell/rshell.c",
    "igris/shell/vterm.c",
    "igris/shell/vtermxx.cpp",
    "igris/sync/syslock.c",
    "igris/sync/critical_context.c",
    "igris/halfer.cpp",
    "igris/osinter/wait.cpp",
    "igris/osinter/ctrobj.cpp"
],
    mdepends=[
    "igris.protocols.gstuff",
    "igris.bug",
    "igris.location"
]
)

licant.module("igris.utilxx", sources=[
    "igris/util/base64.cpp",
    "igris/util/string.cpp",
    "igris/container/dlist.cpp",
    "igris/string/replace.cpp",
    "igris/string/hexascii_string.cpp",
],
    mdepends=[
    "igris.util",
]
)

#################################################################

licant.module("igris.printf_impl", "impl", default=True,
              sources=["igris/util/printf_impl.c"]
              )

licant.module("igris.printf_impl", "stub",
              sources=["igris/util/stubs/printf_stub.c"]
              )

licant.module("igris.syslock", impl="irqs",
              sources=["igris/sync/syslock_irqs.c"])

licant.module("igris.syslock", impl="mutex",
              sources=["igris/sync/syslock_mutex.cpp"], default=True)

licant.module("igris.semaphore", sources=["igris/sync/semaphore.cpp"])

licant.module("igris.protocols.msgtype",
              sources=["igris/protocols/msgtype.cpp"])

licant.module("igris.ctrobj", "linux",
              sources=["igris/osinter/wait-linux.cpp"],
              default=True
              )

#################################################################
licant.module("igris.os_extension", "unix",
              sources=[
                  "igris/osutil/src/posix.cpp",
                  "igris/osutil/realtime.c"
              ],
              default=True
              )

licant.module("igris.os_extension", "windows",
              sources=[
                  #		"igris/osutil/src/osutil_windows.cpp",
                  #		"igris/osutil/realtime.c"
              ],
              default=True
              )

licant.module("igris.crypt.aes",
              sources=["igris/crypt/aes.c"])

licant.module("igris.systime", "jiffies",
              sources=["igris/time/jiffies-systime.cpp",
                       "igris/time/systime.cpp"]
              )

licant.module("igris.systime", "posix",
              sources=["igris/time/time_posix.cpp", "igris/time/systime.cpp"],
              default=True
              )

licant.module("igris.time",
              mdepends=["igris.systime"]
              )

licant.module("igris.flags.clean",
              cxx_flags="-Wl,--gc-sections -fdata-sections -fpermissive -DNDEBUG -fno-threadsafe-statics -ffunction-sections -fno-rtti -flto",
              cc_flags="-Wl,--gc-sections -fdata-sections -DNDEBUG -ffunction-sections -flto",
              ld_flags="-Wl,--gc-sections -fdata-sections -fpermissive -DNDEBUG -fno-threadsafe-statics -ffunction-sections -fno-rtti -flto",
              )

licant.module("igris.location", "dprint", default=True,
              sources=["igris/util/location.c"]
              )

licant.module("igris.location", "stub",
              sources=["igris/util/stubs/location-stub.c"]
              )
