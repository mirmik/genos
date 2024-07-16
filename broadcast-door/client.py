#!/usr/bin/env python3

import socket 
import threading
import signal
import sys
import os
import base64

import argparse

MAXSIZE = 2048
    
parser = argparse.ArgumentParser(description='Broadcast shell client')
parser.add_argument('-i', '--id', help='Unique ID', default=None)
parser.add_argument('-l', '--list', help='List all servers', action='store_true')
parser.add_argument('-c', '--chat', help='Chat with servers', action='store_true')
parser.add_argument('-b', '--broadcast_ip', help='Broadcast IP', default='<broadcast>')
parser.add_argument('args', nargs='*')
args = parser.parse_args()


def signal_handler(signal, frame):
    print()
    os._exit(0)

signal.signal(signal.SIGINT, signal_handler)

client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
client.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
client.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

client.bind(('', 11722))
        

if args.list:
    broadcast_ip = args.broadcast_ip
    client.sendto("ping".encode("utf-8"), (broadcast_ip, 11722))
    
    messages = []

    # listen messages one second
    client.settimeout(1)
    while True:
        try:
            data, addr = client.recvfrom(MAXSIZE)
            messages.append(data)
            msg = data.decode("utf-8")
            if msg.startswith('pong'):
                msglst = msg.split('|')
                msglst.pop(0)
                msg = '|'.join(msglst)
                print(f"{msg} {addr}")
        except socket.timeout:
            break
    exit(0)       

if args.chat:
    def receive():
        while True:
            data, addr = client.recvfrom(MAXSIZE)
            print("Received from %s: %s" % (addr, data))
    
    threading.Thread(target=receive).start()

    while True:
        msg = input()
        client.sendto(msg.encode("utf-8"), ("<broadcast>", 11722))
    

if args.id is None:
    print("Unique ID is required")
    exit(1)
else:
    uid = args.id
    cmd = " ".join(args.args)
    client.sendto(uid.encode("utf-8") + b"|" + cmd.encode("utf-8"), ('255.255.255.255', 11722))

    while True:
        data, addr = client.recvfrom(1024)
        msg = data.decode("utf-8")
        if msg.startswith('answer'):
            break

    msglst = msg.split('|')
    #print(msglst)


    
    output = base64.b64decode(msglst[2])
    if len(msglst) > 3:
        error = base64.b64decode(msglst[3])
    else:
        error = b''
    
    sys.stdout.write(output.decode("utf-8"))
    if len(error) != 0:
        errormsg = error.decode("utf-8")
        sys.stdout.write(errormsg)
