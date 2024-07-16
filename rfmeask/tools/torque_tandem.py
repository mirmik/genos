#!/usr/bin/env python3
#coding: utf-8

import time
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

import socket
import threading
import re

import sys

num = sys.argv[1]
mult = 1 if len(sys.argv) < 3 else (1 / int(sys.argv[2]))

sock = socket.socket()
sock.connect(('192.168.1.23', 5026))
sock.send("NOT:AXIS{}:TORQ 1\n".format(num).encode("utf-8"))

x = np.arange(0,100)
y1 = np.zeros(100)
y2 = np.zeros(100)
idx = 0

fig, ax = plt.subplots()
line1, = ax.plot(x, y1)
line2, = ax.plot(x, y2)
plt.ylim(-100, 100)

pattern = re.compile(r"\[.*\]")
def readfunc():
	global idx
	while True:
		s = ''
		while True:
			c = sock.recv(1).decode("utf-8")
			if c == '\n': break
			s += c

		a = pattern.findall(s)
		for iss in a:
			if idx == 100: idx = 0
			ss = iss[1:-1].split(',')
			pair = (int(ss[0]), int(ss[1]))
			print(pair)
			y1[idx] = pair[0] * mult
			y2[idx] = pair[1] * mult
			idx+=1

thr = threading.Thread(target = readfunc)
thr.start()

def animate(i):
	line1.set_ydata(y1)
	line2.set_ydata(y2)
	return line1, line2


# Init only required for blitting to give a clean slate.
def init():
	line1.set_ydata(y1)
	line2.set_ydata(y2)
	return line1, line2

ani = animation.FuncAnimation(fig, animate, np.arange(1, 200), init_func=init,
							  interval=25, blit=True)
plt.show()