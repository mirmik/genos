#!/usr/bin/env python3

import pycrow

ugate = pycrow.udpgate(18003)
ugate.bind(12)
ugate.debug(True)


def position_handler(message):
    print(message)


pycrow.diagnostic_setup(True)
#sub = pycrow.subscriber(position_handler)

#sub.subscribe(pycrow.crowker_address(), "test_subscriber")
#sub.install_keepalive(1000, True)

pycrow.spin()
