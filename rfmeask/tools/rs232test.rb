#!/usr/bin/ruby
#coding: utf-8

require "serialport"

port = SerialPort.new "/dev/ttyS2", 115200
port.parity= SerialPort::EVEN
port.data_bits= 8
port.stop_bits= 1

p "START MESSAGING"

port.print "\0014000000000E4\003"

loop do
	p port.read 1
end


port.close
