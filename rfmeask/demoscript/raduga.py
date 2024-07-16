#!/usr/bin/env python3

import socket
import time

HOST = '192.168.1.115'
PORT = 5025 

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

print(1)
s.send("AXIS10:USPE 3\r\n".encode("utf-8"))
s.send("AXIS14:USPE 3\r\n".encode("utf-8"))
s.send("AXIS8:USPE 3\r\n".encode("utf-8"))

s.send("AXIS14:UMOV:ABS 20\r\n".encode("utf-8"))
s.send("AXIS10:UMOV:ABS 0\r\n".encode("utf-8"))
s.send("AXIS9:UMOV:ABS 10\r\n".encode("utf-8"))

time.sleep(9)
print("2")
s.send("AXIS14:UMOV:ABS -20\r\n".encode("utf-8"))
s.send("AXIS9:UMOV:ABS 0\r\n".encode("utf-8"))
time.sleep(8)
print("3")
s.send("AXIS11:UMOV:ABS 10\r\n".encode("utf-8"))
s.send("AXIS8:UMOV:ABS 10\r\n".encode("utf-8"))
time.sleep(8)
print("4")
s.send("AXIS11:UMOV:ABS -10\r\n".encode("utf-8"))
s.send("AXIS8:UMOV:ABS -10\r\n".encode("utf-8"))
time.sleep(8)
print("5")
s.send("AXIS11:UMOV:ABS 0\r\n".encode("utf-8"))
s.send("AXIS8:UMOV:ABS 0\r\n".encode("utf-8"))
s.send("AXIS14:UMOV:ABS 0\r\n".encode("utf-8"))
time.sleep(3)
print("6")
s.send("AXIS15:UMOV:ABS 3\r\n".encode("utf-8"))
time.sleep(3)
print("7")
s.send("AXIS15:UMOV:ABS -3\r\n".encode("utf-8"))


time.sleep(5)
s.send("AXIS8:UMOV:ABS 0\r\n".encode("utf-8"))
s.send("AXIS9:UMOV:ABS 0\r\n".encode("utf-8"))
s.send("AXIS10:UMOV:ABS 40\r\n".encode("utf-8"))
s.send("AXIS15:UMOV:ABS 0\r\n".encode("utf-8"))

#s.send("AXIS14:UMOV:ABS -25\r\n".encode("utf-8"))

#time.sleep(5)
#s.send("AXIS8:UMOV:ABS -20\r\n".encode("utf-8"))


print("endscript")
