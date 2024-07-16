#!/usr/bin/env python3 

import socket
import sys
import time

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(("127.0.0.1", 5025))

def axno_to_axname(axno):
	return f"AXIS{axno}"

def request_position(axno):
	axis = axno_to_axname(axno)
	sock.send(f"{axis}:UPOS?\r\n".encode("utf-8"))
	time.sleep(0.1)
	ans = sock.recv(1024).decode("utf-8")
	return float(ans)

def absmove(axno, pos):
	axis = axno_to_axname(axno)
	sock.send(f"{axis}:UMOV:ABS {pos}\r\n".encode("utf-8"))

def absmove_sync(axno, pos):
	absmove(axno, pos)
	wait_moving(axno)

def wait_moving(axno):
	axis = axno_to_axname(axno)
	
	fin = False

	while not fin:
		time.sleep(0.1)
		sock.send(f"{axis}:STAT:OP?\r\n".encode("utf-8"))
		time.sleep(0.1)
		ans = sock.recv(1024).decode("utf-8")
	
		if (int(ans) == 0): 
			fin = True

if request_position(11) > 20:
	absmove_sync(18, 90)
else:
	absmove_sync(18, 0)

wait_moving(9)
wait_moving(10)

# Установка нулей.
absmove_sync(1, 0)
absmove_sync(15, 0)
absmove_sync(12, 0)
absmove_sync(17, 0)
absmove_sync(16, 0)

# Откатить икс.
absmove_sync(13, -25)

# Поворот 90 градусов
absmove_sync(18, 90)

az9 = request_position(9)
az10 = request_position(10)

# Спуск колпака 90 градусов.
if request_position(11) < 20:
	az9 = request_position(9)
	az10 = request_position(10)
	if abs(az9+90) > 3 or abs(az10+90) > 3:
		exit(0)

absmove_sync(11, 90)
