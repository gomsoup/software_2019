#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "main.h"

using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->num0, SIGNAL(clicked()), this, SLOT(myclicked()));
    connect(ui->num1, SIGNAL(clicked()), this, SLOT(myclicked()));
    connect(ui->num2, SIGNAL(clicked()), this, SLOT(myclicked()));
    connect(ui->num3, SIGNAL(clicked()), this, SLOT(myclicked()));
    connect(ui->num4, SIGNAL(clicked()), this, SLOT(myclicked()));
    connect(ui->num5, SIGNAL(clicked()), this, SLOT(myclicked()));
    connect(ui->num6, SIGNAL(clicked()), this, SLOT(myclicked()));
    connect(ui->num7, SIGNAL(clicked()), this, SLOT(myclicked()));
    connect(ui->num8, SIGNAL(clicked()), this, SLOT(myclicked()));
    connect(ui->num9, SIGNAL(clicked()), this, SLOT(myclicked()));
    connect(ui->undo, SIGNAL(clicked()), this, SLOT(myclicked()));
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_num1_clicked()
{
    stdnum.append("1");
}

void MainWindow::myclicked(){
    ui->numbers->setText(stdnum.c_str());
}

void MainWindow::on_num2_clicked()
{
    stdnum.append("2");
}

void MainWindow::on_num3_clicked()
{
    stdnum.append("3");
}

void MainWindow::on_num4_clicked()
{
    stdnum.append("4");
}

void MainWindow::on_num5_clicked()
{
    stdnum.append("5");
}

void MainWindow::on_num6_clicked()
{
    stdnum.append("6");
}

void MainWindow::on_num7_clicked()
{
    stdnum.append("7");
}

void MainWindow::on_num8_clicked()
{
    stdnum.append("8");
}

void MainWindow::on_num9_clicked()
{
    stdnum.append("9");
}

void MainWindow::on_num0_clicked()
{
    stdnum.append("0");
}

void MainWindow::on_undo_clicked()
{
    if(!stdnum.empty())
        stdnum.pop_back();

}

void MainWindow::on_registerButton_clicked()
{
    int ret;

    string txt= "Student number : " + stdnum + "\n" +  "right?";
    QMessageBox msgBox;
    msgBox.setText("Warning");
    msgBox.setInformativeText(txt.c_str());
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);

    ret = msgBox.exec();

    switch (ret){

    case QMessageBox::Yes:{
        switchCamera(stdnum);
        break;
    }
    case QMessageBox::No:
        break;

    default:
        break;
    }

}
