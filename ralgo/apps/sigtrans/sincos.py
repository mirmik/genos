#!/usr/bin/env python3

import zencad
import pycrow
import ctypes
import numpy

pycrow.create_udpgate(12)

X = 0
Y = 0
A = 0

def position_handler(pack):
	global X, Y, A
	data = pack.message()
	ndata = numpy.frombuffer(data, numpy.float32)
	X = ndata[0]
	Y = ndata[1]
	A = float(ndata[2])

pycrow.start_spin()
sub = pycrow.subscriber(position_handler)

sub.subscribe(pycrow.crowker_address(), "sigtrans/feedpos")

b = zencad.disp(zencad.box(20, 10, 10, center= True))

X = 0
Y = 0
A = 0

def animate(arg):
	b.relocate(zencad.movY(X/100) * zencad.movZ(Y/100) * zencad.rotateX(A/100))

zencad.show(animate = animate)
