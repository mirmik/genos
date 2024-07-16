import licant


licant.module("igris.compat.std",
              include_paths=["."],
              )

licant.module("igris.std",
            mdepends=["igris.compat.std"]
              )
