#!/usr/bin/env python3

import socket
import time
import sys
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("--sleeptime", required=True)
parser.add_argument("--ip", required=True)
parser.add_argument("--port", default="5025")
args = parser.parse_args()

IP = args.ip
PORT = int(args.port)
SLEEPTIME = float(args.sleeptime)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((IP, PORT))

s.send("AXIS1:MANTRIG 1\n".encode("utf-8"))
while True:
	s.send("AXIS1:TRIGGER\n".encode("utf-8"))
	time.sleep(SLEEPTIME)