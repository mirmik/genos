#!/usr/bin/env python3

import pycrow
import time

def foo(pack):
	print("hello_python")

pycrow.create_udpgate(12, 10010)
pycrow.start_spin()


pycrow.enable_crowker_subsystem()
pycrow.diagnostic_setup(True, False)

addr = pycrow.address("")

sub = pycrow.subscriber(foo)
sub.subscribe(addr, "hello")

crowker = pycrow.crowker.instance()
crowker.set_info_mode(True)

time.sleep(0.1)
crowker.publish("hello", "data")


pycrow.join_spin()