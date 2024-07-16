#!/usr/bin/ruby
#coding: utf-8

require "socket"

socket = TCPSocket.new "127.0.0.1", 9667 

loop do 

print socket.readline

end

socket.close
