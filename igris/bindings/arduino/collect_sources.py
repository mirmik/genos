#!/usr/bin/env python3

import os
import shutil
import glob

shutil.rmtree("src")
os.makedirs("src/igris", exist_ok=True)

PREFIX = "../../igris"

SOURCES = [
#	"dprint/dprint_func_impl.c",
#	"dprint/dprint_manually.c",
	"sync/syslock.c",

	"shell/mshell.c",

	#"util/bug_abort.c",
	#"util/location.c",
	"util/numconvert.c",
	"util/hexascii.c",
]

for path, subdirs, files in os.walk(PREFIX):
    for f in [ f for f in files if os.path.splitext(f)[1] == ".h" ]:
    	difpath = os.path.relpath(path, PREFIX)
    	os.makedirs(os.path.join("src/igris", difpath), exist_ok=True)
    	shutil.copyfile(src=os.path.join(PREFIX, difpath, f), dst=os.path.join("src/igris", difpath, f))

for s in SOURCES:
	src = os.path.join(PREFIX, s)
	name = os.path.basename(s)

	shutil.copyfile(src, os.path.join("src", name))

shutil.copytree(src="addon", dst="src", dirs_exist_ok=True)

with open("src/igris.h", "w") as f:
	f.write("//PLACEHOLDER\r\n")
