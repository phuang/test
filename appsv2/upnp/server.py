#!/usr/bin/env python
import SimpleHTTPServer
import SocketServer

PORT = 8000

class MyHTTPRequestHandler(SimpleHTTPServer.SimpleHTTPRequestHandler):
  pass

Handler = MyHTTPRequestHandler

httpd = SocketServer.TCPServer(("", PORT), Handler)

print "serving at port", PORT
httpd.serve_forever()
