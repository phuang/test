#!/usr/bin/env python

import socket
import sys
import urllib2

HOST = "239.255.255.250"
PORT = 1900

def m_search():
  data = (
      'M-SEARCH * HTTP/1.1',
      'MX: 5',
      'ST: upnp:rootdevice',
      'MAN: "ssdp:discover"',
      'User-Agent: Platinum/0.5.3.0, DLNADOC/1.50',
      'Host: 239.255.255.250:1900')
  data = "\r\n".join(data) + "\r\n\r\n"
  sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
  # sock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, 2)
  sock.sendto(data, (HOST, PORT))
  received = sock.recv(1024)

  values = {}
  lines = received.split("\r\n")
  for line in lines[1:]:
    if not line:
      continue
    name, value = line.split(":", 1)
    values[name] = value.strip()
  return values

def get_description(values):
  f = urllib2.urlopen(values["LOCATION"])
  for line in f:
    print line,
  


if __name__ == "__main__":
  values = m_search()
  get_description(values)
