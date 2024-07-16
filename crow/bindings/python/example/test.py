#!/usr/bin/env python3

import pycrow

pycrow.start_client()
sub = pycrow.rxsubscribe("lalala2")

sub.subscribe(lambda x : print(x))

while True:
	pass
pycrow.stop_client()