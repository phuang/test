#!/usr/bin/env python

import socket
import sys
import urllib2
import select

HOST = "239.255.255.250"
PORT = 1900

def m_search():
  sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
  sock.bind()

  # Set the socket multicast TTL to 1+, so the multicast package can pass
  # router (optional).
  sock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, 2)

  m_search_package = (
      'M-SEARCH * HTTP/1.1',
      'MX: 5',
      'ST: upnp:rootdevice',
      'MAN: "ssdp:discover"',
      'User-Agent: Platinum/0.5.3.0, DLNADOC/1.50',
      'Host: 239.255.255.250:1900')
  data = "\r\n".join(m_search_package) + "\r\n\r\n"

  sock.sendto(data, (HOST, PORT))
  print "==>"
  print "To: 239.255.255.250:1900"
  print data

  # Wait servers' resopnses.
  while True:
    readyfds = select.select([sock.fileno()], [], [])
    if sock.fileno() in readyfds[0]:
      received, address = sock.recvfrom(1024)
      print "<=="
      print "From: ", address
      print received

  # values = {}
  # lines = received.split("\r\n")
  # for line in lines[1:]:
  #   if not line:
  #     continue
  #   name, value = line.split(":", 1)
  #   values[name] = value.strip()
  # print values

def get_description(values):
  f = urllib2.urlopen(values["LOCATION"])
  for line in f:
    print line,



if __name__ == "__main__":
  values = m_search()
  # get_description(values)
