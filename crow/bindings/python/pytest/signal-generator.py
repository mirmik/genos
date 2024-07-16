#!/usr/bin/env python3

import pycrow
import signal
import sys
import threading
import math
import signal
import time
import argparse

parser = argparse.ArgumentParser(description='Rabbit signal generator')
parser.add_argument('--theme', type=str, default='default', help='Theme name')
parser.add_argument('--freq', type=float, default=50, help='frequency')
parser.add_argument('--pattern', type=str, default='sine', help='pattern')
opts = parser.parse_args()

interval = 1.0 / opts.freq
pattern = opts.pattern

pycrow.create_udpgate(12)
pub = pycrow.publisher(pycrow.crowker_address(), opts.theme)

while True:
	curtime = time.time()
	if pattern == 'sine':
		data = math.sin(curtime)

	elif pattern == 'square':
		data = math.sin(curtime)
		if data > 0:
			data = 1
		else:
			data = -1
	
	elif pattern == 'sawtooth':
		data = curtime % 1
	
	print(int(time.time()*1000), data)
	pub.publish((str(int(time.time()*1000)) + " " + str(data) + str("\n")).encode("utf-8"))
	pycrow.onestep()
	time.sleep(interval)

