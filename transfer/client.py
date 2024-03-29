import socket
import os
import zipfile

HOST = "192.168.0.2"
PORT = 50000
ADDR = (HOST, PORT)
BUFF_SIZE = 1024

filenames = os.listdir("/etc/face/pic")
filenames = [file for file in filenames if file.endswith(".png")]

stdnum = "201324474"
zip = zipfile.ZipFile(stdnum + ".zip", "w")

for file in filenames:
    zip.write(os.path.join("/etc/face/pic", file), file, compress_type = zipfile.ZIP_DEFLATED)
zip.close()


clientSocket = socket.socket()
clientSocket.connect(ADDR)

clientSocket.send("201324474".encode())
clientSocket.recv(1024)

with open(stdnum + ".zip", 'rb') as f:
    print ("file opened")

    print ("sending data...")
    l = f.read(4096)

    while(l):
        clientSocket.send(l)
        l = f.read(4096)
    f.close()

    print("Successfully send the file")

    print("waiting server yml...")
    fd = open("/etc/face/face.yml".encode(), 'wb')
    while True:
        data = clientSocket.read(4096)
        if not data:
            break
        fd.write(data)    
    fd.close()

clientSocket.close()
print('connection closed')
