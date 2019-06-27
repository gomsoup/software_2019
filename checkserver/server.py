import socket
import os
import cv2
import zipfile
import numpy

HOST = socket.gethostname()
PORT = 50001
ADDR = (HOST, PORT)
BUFF_SIZE = 1024

server = socket.socket()

server.bind(ADDR)
server.listen(5)

while True:
    fd, addr = server.accept()

    stdnum = fd.recv(1024).decode()
    print("student : " + stdnum)
    fd.send("ok".encode())

    