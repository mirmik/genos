#!/usr/bin/env python3

import os
import shutil
import glob

os.makedirs("src/genos", exist_ok=True)

PREFIX = "../../genos"

SOURCES = [
	"schedee.c",
	"schedee_api.c",
	"autom_schedee.c",
	"ktimer.c",
	"drivers/serial/arduino_serial_device.cpp",
	"drivers/device_head.c",
	"drivers/devns.c",
	"drivers/virtual/devnull.c",
	"resource/file_head.c",
	"resource/mvfs.c",	
	"resource/namespace.c",
	"resource/openres.c",		
]

for path, subdirs, files in os.walk(PREFIX):
    for f in [ f for f in files if os.path.splitext(f)[1] == ".h" ]:
    	difpath = os.path.relpath(path, PREFIX)
    	os.makedirs(os.path.join("src/genos", difpath), exist_ok=True)
    	shutil.copyfile(src=os.path.join(PREFIX, difpath, f), dst=os.path.join("src/genos", difpath, f))

for s in SOURCES:
	src = os.path.join(PREFIX, s)
	name = os.path.basename(s)

	shutil.copyfile(src, os.path.join("src", name))

with open("src/genos.h", "w") as f:
	f.write("//PLACEHOLDER\r\n")