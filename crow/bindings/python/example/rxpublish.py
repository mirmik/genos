#!/usr/bin/env python3

import time

import pycrow
from pycrow.rxcrow import rxpublish

from rxsignal import rxinterval

pycrow.start_client()
rxpublish("pulse", rxinterval(1).map(lambda t: (str(t)+"\n").encode("utf-8")))

while True:
    time.sleep(1)
