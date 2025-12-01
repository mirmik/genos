import licant


licant.module("igris.compat.std",
              include_paths=["."],
            defines=["IGRIS_STD_AS_STD"]
              )

licant.module("igris.std",
            mdepends=["igris.compat.std"]
              )
