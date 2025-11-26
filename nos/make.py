#!/usr/bin/env python3
#coding: utf-8

import licant
import licant.install
import shutil
import os
import sys

licant.execute("nos.g.py")

target = "libnos.so"
install_include_path = '/usr/include/nos'
install_directory_path = '/usr/lib/'
install_library_path = os.path.join(install_directory_path, target)
install_library_link = os.path.join(install_directory_path, 'libnos.so')

licant.cxx_static_and_shared("libraries",
                             static_lib="libnos.a",
                             shared_lib="libnos.so",
                             mdepends=[
                                 "nos",
                                 "nos.inet",
                                 "nos.input",
                                 "nos.check",
                                 "nos.log"
                             ],
                             cxxstd="c++2a",
                             ccstd="c11",
                             cxx_flags='-fPIC  -Weffc++ -Wall -Wextra',
                             cc_flags='-fPIC ',
                             ld_flags='-fPIC ',
                             libs=["ws2_32"] if sys.platform == "win32" else [],
                             )

licant.cxx_application("runtests",
                       sources=["tests/*.cpp"],
                       objects=["libnos.a"],
                       include_paths=["tests/.", "."],
                       cxxstd="c++2a",
                       ccstd="c11",
                       cxx_flags="-Weffc++ -g -O0 -fPIC -Werror=all -Werror=pedantic -Wno-gnu-zero-variadic-macro-arguments",
                       cc_flags="-g -O0 -fPIC -Werror=all -Werror=pedantic -Wno-gnu-zero-variadic-macro-arguments",
                       ld_flags="-g -O0 -fPIC -L/usr/local/lib/",
                       libs=["pthread"],
                       )

licant.install.install_library(tgt="install",
                               libtgt=["libnos.a", "libnos.so"],
                               headers="nos",
                               hroot="nos")

licant.fileset("all", targets=["runtests", "libnos.a", "libnos.so"])

licant.ex("all")
