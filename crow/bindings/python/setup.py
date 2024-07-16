#!/usr/bin/env python3
#coding: utf-8

from wheel.bdist_wheel import bdist_wheel as bdist_wheel_
from setuptools import setup, Extension, Command
from distutils.util import get_platform
import os
import glob
import licant

licant.include("nos", local_tunel=("pycrow/__tunels__/nos", "nos.g.py"))
licant.include("igris", local_tunel=("pycrow/__tunels__/igris", "igris.g.py"))
licant.include("crow", path="../../crow.g.py")
licant.cxx_objects("crow-objects",
                   mdepends=[
                       "crow",
                       "crow.udpgate",
                       "crow.crowker",
                       "nos",
                       "nos.inet",
                       "igris",
                       "igris.syslock",
                       "igris.dprint",
                       ("igris.ctrobj", "linux")
                   ]
                   )

crowopts = licant.default_core().get("crow-objects").finalopts


class bdist_wheel(bdist_wheel_):
    def finalize_options(self):
        from sys import platform as _platform
        platform_name = get_platform()

        if _platform == "linux" or _platform == "linux2":
            # Linux
            if platform_name == "linux-i686":
                platform_name = 'manylinux1_i686'
            else:
                platform_name = 'manylinux1_x86_64'

        bdist_wheel_.finalize_options(self)
        self.universal = True
        self.plat_name_supplied = True
        self.plat_name = platform_name


pycrow_lib = Extension("pycrow.libcrow",
                       sources=[
                           "requestor.cpp",
                           "subscriber.cpp",
                           "publisher.cpp",
                           "pywrap.cpp",
                       ] + crowopts["sources"],
                       extra_compile_args=['-fPIC', '-std=c++17', '-DCROW_USE_ASYNCIO=1'],
                       extra_link_args=['-Wl,-rpath,$ORIGIN/libs'],
                       include_dirs=crowopts["include_paths"],
                       libraries=[],
                       )

setup(
    name='pycrow',
    packages=['pycrow'],
    version='0.1.5',
    license='MIT',
    description='Messaging system',
    author='Sorokin Nikolay',
    author_email='mirmikns@yandex.ru',
    url='https://mirmik.github.io/crow/',
    keywords=['testing', 'cad'],
    classifiers=[],

    ext_modules=[pycrow_lib],
    cmdclass={"bdist_wheel": bdist_wheel},

    package_data={"pycrow": [
        "__tunels__/nos/nos.g.py",
        "__tunels__/igris/igris.g.py"
        "__tunels__/igris/compat/libc/libc.g.py",
        "__tunels__/igris/compat/std/std.g.py",
        "__tunels__/igris/compat/posix/posix.g.py",
    ]},
    include_package_data=True,
)
