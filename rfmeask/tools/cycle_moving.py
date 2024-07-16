#!/usr/bin/env python3

import socket
import time
import sys

# connect to 192.168.1.217
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(("192.168.1.217", 5025))


def axno_to_axname(axno):
    return f"AXIS{axno}"


def request_position(axno):
    axis = axno_to_axname(axno)
    sock.send(f"{axis}:UPOS?\r\n".encode("utf-8"))
    time.sleep(0.1)
    ans = sock.recv(1024).decode("utf-8")
    return float(ans)


def set_speed(axno, spd):
    axis = axno_to_axname(axno)
    sock.send(f"{axis}:SPE {spd}\r\n".encode("utf-8"))


def set_uspeed(axno, spd):
    axis = axno_to_axname(axno)
    sock.send(f"{axis}:USPE {spd}\r\n".encode("utf-8"))


def absmove(axno, pos):
    axis = axno_to_axname(axno)
    sock.send(f"{axis}:UMOV:ABS {pos}\r\n".encode("utf-8"))


def absmove_sync(axno, pos, info=False):
    start_time = time.time()
    absmove(axno, pos)
    wait_moving(axno)
    finish_time = time.time()
    if info:
        print(f"Moving time: {finish_time - start_time} sec")


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


axno = 6
set_speed(axno, 500)
iter = 0
while 1:
    absmove_sync(axno, 300, info=True)
    absmove_sync(axno, -300, info=True)
    iter += 1
