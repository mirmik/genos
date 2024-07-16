#!/usr/bin/env python3

import sys
import time
import socket
import threading

HOST = '192.168.1.224' # ip адрес изделия
SCPI_PORT = 5025 
NCPI_PORT = 5026

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
n = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, SCPI_PORT))
n.connect((HOST, NCPI_PORT))

# запросить идентификатор
s.send("AXIS0:STAT:IDN?\r\n".encode("utf-8"))
answer = s.recv(1024).decode("utf-8")
print("axis0 idn is {}".format(answer))

print("enable notification")
n.send("NOT:AXIS0:UPOS SMOOTH, 0.1\r\n".encode("utf-8"))
n.send("NOT:AXIS0:OPSTAT 1\r\n".encode("utf-8"))

cancel = False
def foo():
	n.settimeout(0.5)
	while cancel is False:
		try:
			message = n.recv(1024).decode("utf-8")
		except:
			pass
		sys.stdout.write("notification: {}".format(message)) 

thr = threading.Thread(target=foo)
thr.start()

# переместится в координату 4 юнита 
print("move to 4u")
s.send("AXIS0:UMOV:ABS 4\r\n".encode("utf-8"))

time.sleep(5)

# переместится в координату 0 юнитов
print("move to 0u")
s.send("AXIS0:UMOV:ABS 0\r\n".encode("utf-8"))

time.sleep(5)
cancel = True