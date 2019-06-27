#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QProcess>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <iostream>
#include <vector>
#include <thread>

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/face.hpp"

#include "v4l_wrapper.h"
#include "main.h"
#define WIDTH 1024
#define HEIGHT 600

using namespace std;
using namespace cv;
fsl_v4l_cap mycamera;
fsl_v4l_out mydisplay;

char * buffer = (char *) malloc (640 * 480 * 2);
int cnt = 0; bool isTrain = true;
auto model = face::createLBPHFaceRecognizer(1, 8, 8, 8, 3000.0);
string studnumber = "201324474";

int Kbhit (void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}


static int dbread(const string& filename, vector<Mat>& images, vector<int>& studnums, char separator = ';'){
    ifstream file(filename);

    if (!file){
        string error = "no valid input file";
        cout << error << endl;
        return 0;
    }

    string line, path, label, studnum;
    while (!file.eof())
    {
        getline(file, line);
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, studnum, separator);
        if (!path.empty() && !studnum.empty()){
            images.push_back(imread(path));
            studnums.push_back(atoi(studnum.c_str()));
        }
    }
}

vector<Mat> faceDetect(Mat inp){
    Mat gray;
    vector <Rect> faces, crop;
    vector <Mat> ret;
    cvtColor(inp, gray, COLOR_BGR2GRAY);
    CascadeClassifier cas;
    //string path = "/usr/share/OpenCV/haarcascades/haarcascade_frontalface_default.xml";

    string path = "/usr/share/OpenCV/lbpcascades/lbpcascade_frontalface.xml";
    if(! cas.load(path) ){
        cerr << "haarcascade load failed" << endl;
        exit(0);
    }

    cas.detectMultiScale(gray, faces, 1.1, 3, 0 | CV_HAAR_SCALE_IMAGE, Size(50, 50), Size(150,150));

    for(int i = 0; i < faces.size(); i++){
        Point lb(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
        Point tr(faces[i].x, faces[i].y);

        Mat roi(inp, Rect(faces[i].x, faces[i].y, faces[i].width, faces[i].height));
        Mat result;
        roi.copyTo(result);

        ret.push_back(result);
    }


    return ret;
}


void recognize(){

    string dbpath = "/etc/face/db.txt";
    ofstream dbout(dbpath, ios::app);

    while(true){
    Mat inp(Size(640,480), IPL_DEPTH_8U, buffer);
    Mat tmp;


    cvtColor(inp, tmp, COLOR_YUV2RGB_UYVY);
    vector <Mat> out = faceDetect(tmp);
    if(!out.empty() && cnt < 20){
        cnt++;
        Mat res = out[0];
        resize(res, res, Size(200, 200), 0, 0, CV_INTER_LINEAR);
        cvtColor(res, res, COLOR_BGR2GRAY);

        string path = "/etc/face/pic/" + to_string(cnt) + ".png";
        dbout << path << ";" << studnumber << endl;
        cout << path << endl;
        imwrite(path, res);
    }
    if(cnt >= 20) break;
    }

    dbout.close();
}


void trainData(){
    //FileStorage fs("/etc/face/face.yml", FileStorage::READ);

    //model->read(fs["opencv_lbphfaces"]);
    model->load("/etc/face/face.yml");
    cout << "train data loaded" << endl;

    cout << "capturing..." << endl;
    V4LWrapper_CreateCameraCapture (&mycamera, "/dev/video0", 640, 480);
    V4LWrapper_CreateOutputDisplay (&mydisplay, "/dev/fb0", NULL, 640, 480);

    while(!Kbhit()){
        V4LWrapper_OutputDisplay (&mydisplay, buffer);
        Mat inp(Size(640,480), IPL_DEPTH_8U, buffer), tmp;

        cvtColor(inp, tmp, COLOR_YUV2RGB_UYVY);
        vector <Mat> out = faceDetect(tmp);

        if(!out.empty()){
            Mat res;

            cvtColor(out[0], res, COLOR_BGR2GRAY);

            int label = -1; double confidence;
            model->predict(res, label, confidence);

            string display = to_string(confidence) + "% Confidence it is user";
            putText(inp, display, Point(100,120), FONT_HERSHEY_COMPLEX, 1.2, Scalar::all(255));
            cout << display << endl;

            if(label == -1){
                putText(inp, "unknown", Point(250, 450), FONT_HERSHEY_COMPLEX, 1.2, Scalar(0, 255, 0));
                cout << "unknown" << endl;
            }
            else{
                 putText(inp, to_string(label), Point(250,450), FONT_HERSHEY_COMPLEX, 1.2, Scalar(0,255,0));
                cout << label << endl;
            }
        }
        else{
           // cout << "face not detected" << endl;
            putText(inp, "face not found", Point(250,450), FONT_HERSHEY_COMPLEX, 1.2, Scalar(0,255,0));
            cout << "not found" << endl;
        }

        V4LWrapper_QueryFrame (&mycamera, (char *)inp.data);
    }


    V4LWrapper_CloseCameraCapture (&mycamera);
    V4LWrapper_CloseOutputDisplay (&mydisplay);
}

void detectAndDisplay(Mat& frame)
{

    std::vector<Rect> faces;
    Mat frame_gray;

    CascadeClassifier cas;
    string path = "/usr/share/OpenCV/lbpcascades/lbpcascade_frontalface.xml";
    cas.load(path);

    Mat inp(Size(640,480), IPL_DEPTH_8U, buffer);
    Mat tmp;


    cvtColor(inp, tmp, COLOR_YUV2RGB_UYVY);
    cvtColor(tmp, frame_gray, COLOR_RGB2GRAY);
    equalizeHist(frame_gray, frame_gray);

    cas.detectMultiScale(frame_gray, faces, 1.1, 2, CV_HAAR_FIND_BIGGEST_OBJECT | CV_HAAR_SCALE_IMAGE, Size(30, 30), Size(100, 100));

    for (size_t i = 0; i < faces.size(); i++)
    {
        Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
        ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2),
                0, 0, 360, Scalar(0, 0, 255), 4, 8, 0);
    }


}


void playCamera(){
    V4LWrapper_CreateCameraCapture (&mycamera, "/dev/video0", 640, 480);
    V4LWrapper_CreateOutputDisplay (&mydisplay, "/dev/fb0", NULL, 640, 480);

    while(!Kbhit() && (cnt <= 100) && isTrain){


        V4LWrapper_OutputDisplay (&mydisplay, buffer);

        Mat inp(Size(640,480), IPL_DEPTH_8U, buffer);
        Mat tmp, frame_gray;


        cvtColor(inp, tmp, COLOR_YUV2RGB_UYVY);
        cvtColor(tmp, frame_gray, COLOR_RGB2GRAY);

        /*
        string text = "hida";
        if(cnt < 20 && cnt > 0) text = "Detected face : " + to_string(cnt);
        else if (isTrain) text = "Now training..";
        else text = "Trying to detect face";

        putText(inp, text, Point(10, 40), 2, 1.2, Scalar::all(255));
        */

        string path = "/etc/face/pic/" + to_string(cnt) + ".png";
        imwrite(path, frame_gray);

        cnt ++;
        cout << cnt << endl;
        //detectAndDisplay(inp);
        V4LWrapper_QueryFrame (&mycamera, (char *)inp.data);
    }

    cout << "play camera end" << endl;
    V4LWrapper_CloseCameraCapture (&mycamera);
    V4LWrapper_CloseOutputDisplay (&mydisplay);
}

int switchCamera(){

    cout << "Start switchCamera " << endl;

    cnt = 0; isTrain = true;
    cout << "start play camera" << endl;
    //thread t(playCamera);

    cout << "start face detect" << endl;
    //thread t2(recognize);
    //t2.join();

    //t.join();
    cout << "start train" << endl;

    string line = "python /root/client.py " + studnumber;
    system(line.c_str());

    cout << "hida" << endl;
    while(true){
        ifstream inp("/etc/face/status");

        if(!inp.is_open()){
            cout << "file not exists";
            sleep(1);
            inp.close();
            continue;
        }
        else{
            string tmp;
            inp >> tmp;
            cout << tmp << endl;
            if(tmp == "alldone")
                break;
            inp.close();
            sleep(1);
            continue;
        }
    }

    cout << "all done" << endl;

    return 0;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    Q_INIT_RESOURCE(res);

    w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    w.show();

    return a.exec();
}
