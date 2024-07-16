#!/usr/bin/env python3

import zencad
import pycrow
import ctypes
import numpy

pycrow.create_udpgate(12)

X = 0
Y = 0
Z = 0

def position_handler(message):
	#print("position handler")
	global X, Y, Z
	data = message
	try:
		ndata = numpy.frombuffer(data, numpy.int64)
		X = ndata[0]
		Y = ndata[1]
		Z = ndata[2]
	except:
		pass
	#print(X,Y,Z)

#pycrow.diagnostic_setup(True)
pycrow.start_spin()
sub = pycrow.subscriber(position_handler)
sub.bind()

sub.subscribe(pycrow.crowker_address(), "cncsim/poses_bin")
sub.install_keepalive(1000, True)


sphere = zencad.disp(zencad.sphere(10))

def animate(arg):
	sphere.relocate(zencad.movX(X) * zencad.movY(Y) * zencad.movZ(Z))

zencad.show(animate = animate)
