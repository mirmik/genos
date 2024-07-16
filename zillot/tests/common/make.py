#!/usr/bin/env python3

import licant
from licant.cxx_modules import application
from licant.modules import submodule, module
from licant.libs import include

licant.include("igris")
licant.include("zillot", "../../zillot.g.py")

application("runtests",
            sources=["*.cpp"],
            ld_flags="-L/usr/local/lib/",
            include_paths=["."],
            mdepends=[
                "zillot",
                "zillot.mock",
                "igris"
            ],

            cxxstd="c++2a -Weffc++",
            ccstd="c11",
            cxx_flags="-g -Werror=all -Werror=pedantic -Wno-gnu-zero-variadic-macro-arguments",
            cc_flags="-g -Werror=all -Werror=pedantic -Wno-gnu-zero-variadic-macro-arguments",
            defines=["__NOARCH=1"]
            )

if __name__ == "__main__":
    licant.ex("runtests")
