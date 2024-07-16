#!/usr/bin/env python3
#coding: utf-8

import sys
import time
import pycrow

import numpy

import threading

pycrow.set_crowker(".12.109.173.108.206:10009")
pycrow.create_udpgate(12, 10009)

thr = threading.Thread(target=pycrow.spin)
thr.start()

while 1:
	time.sleep(1)

	arr = numpy.array([256,24])
	arr = arr.astype(numpy.float32)
	bts = arr.tobytes()

	pycrow.publish("fltflt", bts)