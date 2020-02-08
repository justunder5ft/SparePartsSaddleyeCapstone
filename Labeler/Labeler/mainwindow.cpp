#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <QPixmap>
#include <QLabel>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    std::string file_location = "C:\\Users\\ephra\\Pictures\\This folder is cursed\\Mr Baxter";

    ui->setupUi(this);
    QPixmap video_frame(file_location.c_str());
    ui->label_frame->setPixmap(video_frame.scaled(1280, 720, Qt::KeepAspectRatioByExpanding));
}

//Dirt
//Gravel
//Sidewalk
//Asfault

//Wet
//Dry

//Snow??
//Ice??

MainWindow::~MainWindow()
{
    delete ui;
}

