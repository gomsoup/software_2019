import socket
import os
import cv2
import zipfile
import numpy
from PIL import Image

cntt = 0

def faceDetect(img):
    cas = cv2.CascadeClassifier("./haarcascade_frontalface_default.xml")
    faces = cas.detectMultiScale(img, 1.1, 5, minSize=(30,30))

    if faces is():
        return None    

    for(x,y,w,h) in faces:
        cropped_face = img[y:y+h, x:x+w]

    return cropped_face

def getImagesWithID(path):
    faces = []
    labels = []

    filenames = os.listdir(path)
    filenames = [file for file in filenames if file.endswith(".png")]

    try:
        if not(os.path.isdir(path + "crop")):
            os.makedirs(os.path.join(path + "crop"))
    except OSError as e:
        if e.errno != errno.EEXIST:
            print("Failed to create directory!!!!!")
            raise

    cntt = 0
    for file in filenames:
        print (file)
        img = Image.open(path + file).convert('L')

        img_numpy = numpy.array(img, 'uint8')

        face = faceDetect(img_numpy)
        if face is not None:
            face = cv2.resize(face, (200,200))
            cv2.imwrite(path + "/crop/" + str(cntt) + ".png", face)
            cntt += 1
        else:
            continue
    

def getAllImages():
    faces = []
    labels = []

    dirnames = os.listdir("./pics")
    print dirnames
    
    for dir in dirnames:
        print dir
        path = "./pics/" + dir + "/crop"
        filenames = os.listdir(path)
        print (filenames)
        filenames = [file for file in filenames if file.endswith(".png")]

        for file in filenames:
            face = cv2.imread(path + "/" + file, cv2.IMREAD_GRAYSCALE)
            faces.append(numpy.asarray(face, dtype=numpy.uint8))
            labels.append(int(dir))

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
        if not(os.path.isdir("./pics/" + stdnum)):
            os.makedirs(os.path.join("./pics/" + stdnum))
    except OSError as e:
        if e.errno != errno.EEXIST:
            print("Failed to create directory!!!!!")
            raise

    path = "./pics/" + stdnum.decode() + "/" + stdnum.decode() + ".zip"
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
    zip = zipfile.ZipFile(path)
    zip.extractall("./pics/" + stdnum.decode())
    zip.close()
    print('unzip end') 

    stdnum = int(stdnum.decode())
  
#if __name__ == "__main__":
    print("crop faces...")
    path = "./pics/" + str(stdnum) + "/" # + "faces" "\\"
    recognizer = cv2.face.createLBPHFaceRecognizer()
    #recognizer = cv2.face.LBPHFaceRecognizer_create()

    getImagesWithID(path)
    faces, labels = getAllImages()

    labels = numpy.asarray(labels, dtype=numpy.int32)
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
