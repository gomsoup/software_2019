import socket
import sys

HOST = "192.168.0.2"
PORT = 50001
ADDR = (HOST, PORT)

fd = socket.socket()
fd.connect(ADDR)

fd.send(sys.argv[1].encode())
fd.recv(1024)

fd.close()
