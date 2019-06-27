#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "main.h"
#include <QImage>
#include <QPixmap>
#include <iostream>
#include <fstream>
#include <QMessageBox>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QImage *img = new QImage();
    QPixmap * buffer = new QPixmap();

    if(img->load(":/image/pnulogo.png")){
        cout << "load image" << endl;
        *buffer = QPixmap::fromImage(*img);
        *buffer = buffer->scaled(img->width(), img->height());
    }else {
        cout << "load logo failed" << endl;
    }

    ui->logo->setPixmap(*buffer);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    string result;

    system("/root/check2 -qws");


    while(true){
        ifstream inp("/etc/face/result");
        if (!inp.is_open()){
            continue;
        }
        else {
            inp >> result;
        }
    }
    cout << "return to main" << endl;
    system("rm -rf /etc/face/result");

    string txt = result + " attendance";

    showMsgbox(txt);
}


void MainWindow::on_pushButton_2_clicked()
{

    system("/root/register -qws");


    while(true){
        ifstream inp("/etc/face/camera");
        if (!inp.is_open()){
            continue;
        }
        else{
            string tmp;
            inp >> tmp;
            if(tmp == "ok")
                break;
        }
    }
    system("rm -rf /etc/face/camera");
    this->show();
    this->update();
}
