#!/usr/bin/env python3

import os
import shutil
import glob

os.makedirs("src/crow", exist_ok=True)

PREFIX = "../../crow"

SOURCES = [
	"src/packet.cpp",
	"src/packet_ptr.cpp",
	"src/tower.cpp",
	"src/gateway.cpp",
	"src/variants/systime.cpp",
	"src/variants/allocation_malloc.cpp",
	"src/variants/warn-stub.cpp",
]

for path, subdirs, files in os.walk(PREFIX):
    for f in [ f for f in files if os.path.splitext(f)[1] == ".h" ]:
    	difpath = os.path.relpath(path, PREFIX)
    	os.makedirs(os.path.join("src/crow", difpath), exist_ok=True)
    	shutil.copyfile(src=os.path.join(PREFIX, difpath, f), dst=os.path.join("src/crow", difpath, f))

for s in SOURCES:
	src = os.path.join(PREFIX, s)
	name = os.path.basename(s)

	shutil.copyfile(src, os.path.join("src", name))

#shutil.copytree(src="addon", dst="src", dirs_exist_ok=True)

with open("src/crow.h", "w") as f:
	f.write("//PLACEHOLDER\r\n")