#!/usr/bin/env python3

import zencad
import zencad.assemble
import pycrow
import ctypes
import numpy

pycrow.create_udpgate(12)

A = 0
B = 0
C = 0
D = 0

def position_handler(pack):
	global A,B,C,D
	data = pack.message()
	ndata = numpy.frombuffer(data, numpy.float32)
	A = ndata[0]
	B = ndata[1]
	C = ndata[2]
	D = ndata[3]

pycrow.start_spin()
sub = pycrow.subscriber(position_handler)

sub.subscribe(pycrow.crowker_address(), "sigtrans/feedpos")

cyl = zencad.cylinder(r=2, h=10)

ar = zencad.assemble.rotator([0,0,1])
br = zencad.assemble.rotator([1,0,0], location=zencad.movZ(10))
cr = zencad.assemble.rotator([1,0,0], location=zencad.movZ(10))
dr = zencad.assemble.rotator([1,0,0], location=zencad.movZ(10))

au = zencad.assemble.unit(parts=[cyl])
bu = zencad.assemble.unit(parts=[cyl])
cu = zencad.assemble.unit(parts=[cyl])
du = zencad.assemble.unit(parts=[cyl])

ar.link(au)
br.link(bu)
cr.link(cu)
dr.link(du)

au.link(br)
bu.link(cr)
cu.link(dr)

zencad.disp(ar)

def animate(arg):
	ar.set_coord(A)
	br.set_coord(B)
	cr.set_coord(C)
	dr.set_coord(D)
	#print(A,B,C,D)
	ar.location_update()


zencad.show(animate = animate)
