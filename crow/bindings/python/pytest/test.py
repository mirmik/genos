#!/usr/bin/env python3
#coding: utf-8

import sys
import pycrow

def foo(data):
	print(data.decode("utf-8"))
	pycrow.stop_spin(False)

addr = pycrow.address(".12.127.0.0.1:10009")
ugate = pycrow.udpgate()
ugate.bind(12)
ugate.open(10010)
pycrow.start_spin()

subs = pycrow.subscriber(foo)
subs.subscribe(addr, "hello")

pub = pycrow.publisher(addr, "hello")
pub.publish("Test passed".encode("utf-8"))

pycrow.join_spin()