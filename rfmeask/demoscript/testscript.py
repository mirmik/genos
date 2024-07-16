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

# установить скорость 1 юнит в секунду
s.send("AXIS0:USPE 1\r\n".encode("utf-8"))

# установить интервал ускорения до номинала 2 секунды 
s.send("AXIS0:ACCEL 2000\r\n".encode("utf-8"))

# переместится в координату 4 юнита 
print("move to 4u")
s.send("AXIS0:UMOV:ABS 4\r\n".encode("utf-8"))

time.sleep(5)

# переместится в координату 0 юнитов
print("move to 0u")
s.send("AXIS0:UMOV:ABS 0\r\n".encode("utf-8"))
