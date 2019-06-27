import socket
import os
import cv2
import zipfile
import numpy
from PIL import Image

def faceDetect(img):
    cas = cv2.CascadeClassifier("./haarcascade_frontalface_default.xml")
    faces = cas.detectMultiScale(img, 1.3, 5)

    if faces is():
        return None    
    
    for(x,y,w,h) in faces:
        #해당 얼굴 크기만큼 cropped_face에 잘라 넣기 
        #근데... 얼굴이 2개 이상 감지되면??
        #가장 마지막의 얼굴만 남을 듯
        cropped_face = img[y:y+h, x:x+w]
    #cropped_face 리턴 
    return cropped_face

def getImagesWithID(path):
    faces = []
    labels = []

    filenames = os.listdir(path)
    filenames = [file for file in filenames if file.endswith(".png")]

    for file in filenames:
        print (file)
        img = Image.open(path + file).convert('L')

        img_numpy = numpy.array(img, 'uint8')

        face = faceDetect(img_numpy)
        if face is not None:
            face = cv2.resize(face, (200,200))
            faces.append(face)
            labels.append(stdnum)
        else:
            continue
    
    print (faces)
    print (labels)
    return faces, labels


HOST = socket.gethostname()
PORT = 50000
ADDR = (HOST, PORT)
BUFF_SIZE = 1024

serverSocket = socket.socket()

serverSocket.bind(ADDR)
serverSocket.listen(5)


while True:
    print ('Server waiting...')
    clientSocket, addr = serverSocket.accept()
    print ('Connection from', addr)
    
    stdnum = clientSocket.recv(1024)
    print('client : ' + stdnum.decode())
    clientSocket.send("ok".encode())

    size = int(clientSocket.recv(1024))
    print('filesize : ' + str(size))
    clientSocket.send('ok'.encode())

    try:
        if not(os.path.isdir(stdnum)):
            os.makedirs(os.path.join(stdnum))
    except OSError as e:
        if e.errno != errno.EEXIST:
            print("Failed to create directory!!!!!")
            raise

    path = stdnum.decode() + "\\" + stdnum.decode() + ".zip"
    print("filename : " + path)

    with open(path.encode(), 'wb') as f:
        
            print ('file opened')
            print ('receiving data...')
        
            while True:
                data = clientSocket.recv(4096)
                if not data:
                    break
                f.write(data)

            f.close()
    print('Successfully get the file')

    print('unzip start')
    zip = zipfile.ZipFile(stdnum.decode() + "\\" + stdnum.decode() + ".zip")
    zip.extractall(stdnum.decode() + "\\")
    zip.close()
    print('unzip end') 

    stdnum = int(stdnum.decode())
  
#if __name__ == "__main__":
    print("crop faces...")
    path = str(stdnum) + "\\" # + "faces" "\\"
    recognizer = cv2.face.LBPHFaceRecognizer_create()

    faces, labels = getImagesWithID(path)

    print("now training..")
    recognizer.train(faces, numpy.array(labels))
    recognizer.save("./train.yml")

    print("training done. send yml...")
    fd = open("./train.yml", "rb")

    data = fd.read(4096)
    while (data):
        clientSocket.send(data)
        data = fd.read(4096)
    fd.close()
    print ("yml sended")

    clientSocket.close()
    print('connection closed')
