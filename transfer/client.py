import socket
import os
import zipfile

HOST = "192.168.0.2"
PORT = 50000
ADDR = (HOST, PORT)
BUFF_SIZE = 1024

filenames = os.listdir("./pic")
filenames = [file for file in filenames if file.endswith(".bmp")]

stdnum = "201324474"
zip = zipfile.ZipFile(stdnum + ".zip", "w")

for file in filenames:
    zip.write(os.path.join("./pic", file), file, compress_type = zipfile.ZIP_DEFLATED)
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

clientSocket.close()

"""
for path in filenames:
    clientSocket = socket.socket()
    clientSocket.connect(ADDR)
    full_filename = os.path.join("./pic", path)
    print (path)
    clientSocket.send("201324474".encode())
    clientSocket.recv(1024)
    clientSocket.send((path).encode("UTF-8"))
    clientSocket.recv(1024)

    with open(full_filename.encode(), 'rb') as f:
        print ('file opened')
        
        print('sending data...')
        l = f.read(4096)
        while (l):
            clientSocket.send(l)
            l = f.read(4096)
    f.close()

    print('Successfully send the file')
    clientSocket.close()
"""
print('connection closed')