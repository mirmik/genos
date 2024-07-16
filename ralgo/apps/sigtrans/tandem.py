#!/usr/bin/env python3

import zencad
import pycrow
import ctypes
import numpy

pycrow.create_udpgate(12)

X = 0
Y = 0

def position_handler(pack):
	global X, Y
	data = pack.message()
	ndata = numpy.frombuffer(data, numpy.float32)
	X = ndata[0]
	Y = ndata[1]

pycrow.start_spin()
sub = pycrow.subscriber(position_handler)

sub.subscribe(pycrow.crowker_address(), "sigtrans/feedpos")


sphere = zencad.disp(zencad.sphere(10))


X = 10
Y = 20

def animate(arg):
	sphere.relocate(zencad.movY(X) * zencad.movZ(Y))


zencad.show(animate = animate)
