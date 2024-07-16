#!/usr/bin/env python3
#coding: utf-8

import serial

port = serial.Serial("/dev/ttyS3", 115200, timeout=1, parity=serial.PARITY_EVEN, bytesize=8, stopbits=1)

btes = "\001101\00200\003F7".encode("utf-8")
#btes = "\001201\00200\003F8".encode("utf-8")
#btes = "\001301\00200\003F9".encode("utf-8")

print(btes)
port.write(btes)

while True:
	print(port.read(1))

port.close()
