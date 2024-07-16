#!/usr/bin/env python3

import os
import shutil
import glob

if os.path.exists("src"):
	shutil.rmtree("src")
os.makedirs("src/nos", exist_ok=True)

PREFIX = "../../nos"

SOURCES = [
	"*.cpp",
	"print/*.cpp",
	"fprint/*.cpp",
]

for path, subdirs, files in os.walk(PREFIX):
    for f in [ f for f in files if os.path.splitext(f)[1] == ".h" ]:
    	difpath = os.path.relpath(path, PREFIX)
    	os.makedirs(os.path.join("src/nos", difpath), exist_ok=True)
    	shutil.copyfile(src=os.path.join(PREFIX, difpath, f), dst=os.path.join("src/nos", difpath, f))

for s in SOURCES:
	if "*" in s:
		s = glob.glob(os.path.join(PREFIX, s))
		for l in s:
			src = l
			name = os.path.basename(l)	
			shutil.copyfile(src, os.path.join("src", name))
		continue

	src = os.path.join(PREFIX, s)
	name = os.path.basename(s)	
	shutil.copyfile(src, os.path.join("src", name))
	
#shutil.copytree(src="addon", dst="src", dirs_exist_ok=True)

with open("src/nos.h", "w") as f:
	f.write("//PLACEHOLDER\r\n")