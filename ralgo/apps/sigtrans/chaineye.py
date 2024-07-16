#!/usr/bin/env python3

import zencad
import pycrow
import ctypes
import numpy

pycrow.create_udpgate(12)

X = 0
Y = 0
Z = 0

def position_handler(pack):
	global X, Y, Z
	data = pack.message()
	ndata = numpy.frombuffer(data, numpy.float32)
	X = ndata[0]
	Y = ndata[1]
	Z = ndata[2]

pycrow.start_spin()
sub = pycrow.subscriber(position_handler)

sub.subscribe(pycrow.crowker_address(), "sigtrans/feedpos")


sphere = zencad.disp(zencad.sphere(10))

def animate(arg):
	sphere.relocate(zencad.movX(X) * zencad.movY(Y) * zencad.movZ(Z))


zencad.show(animate = animate)
