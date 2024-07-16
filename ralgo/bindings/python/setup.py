#!/usr/bin/env python3
#coding: utf-8

from wheel.bdist_wheel import bdist_wheel as bdist_wheel_
from setuptools import setup, Extension, Command
from distutils.util import get_platform
import os

import licant

licant.include("nos", local_tunel=("build/nos", "nos.g.py"))
licant.include("igris", local_tunel=("build/igris", "igris.g.py"))
licant.include("linalg", local_tunel=("build/linalg", "linalg.g.py"))
licant.include("ralgo", "../../ralgo.g.py")

licant.cxx_objects("ralgo-objects", 
	mdepends = [
		"ralgo",
		"nos",
		"igris",
		"nos.current_ostream"
	] 
)
ralgoopts = licant.core.core.get("ralgo-objects").finalopts

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

ralgo_lib = Extension("ralgo.libralgo",
	sources = [
		"pywrap.cpp",
		"heimer.cpp",
	] + ralgoopts["sources"],
	extra_compile_args=['-fPIC', '-std=c++17'],
	extra_link_args=['-Wl,-rpath,$ORIGIN/libs'],
	include_dirs = ralgoopts["include_paths"],
	libraries = [],
)

setup(
	name = 'ralgo',
	packages = ['ralgo'],
	package_dir = {'ralgo': 'pyralgo'},
	version = '0.0.1',
	license='MIT',
	description = '???????????',
	author = 'Sorokin Nikolay',
	author_email = 'mirmikns@yandex.ru',
	url = 'https://mirmik.github.io/ralgo/',
	keywords = ['testing', 'cad'],
	classifiers = [],

	include_package_data=True,
	ext_modules = [ralgo_lib],
	cmdclass = {"bdist_wheel" : bdist_wheel}
)
