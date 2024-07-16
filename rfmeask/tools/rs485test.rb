#!/usr/bin/ruby
#coding: utf-8

require "serialport"

port = SerialPort.new "/dev/ttyS3", 115200
port.parity= SerialPort::EVEN
port.data_bits= 8
port.stop_bits= 1

port.print "\001201\00200\003F8"

loop do
	p port.read 1
end


port.close
