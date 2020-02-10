#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>
#include <QString>
#include <QDir>
#include <QPixmap>
#include <QLabel>
#include <fstream>
#include <QCheckBox>
#include <QMediaPlayer>
#include <QVideoProbe>
#include <QDebug>
#include <QAbstractVideoSurface>
#include <QVideoWidget>
#include <QMediaPlaylist>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    std::string file_location = "C:/Users/ephra/Pictures/This folder is cursed/Cory using is special laser vision glasses just like cyclops from the xmen";

    ui->setupUi(this);
    QPixmap video_frame(file_location.c_str());
    ui->label_frame->setPixmap(video_frame.scaled(1280, 720, Qt::KeepAspectRatioByExpanding));

    player = new QMediaPlayer;
    videoWidget = new QVideoWidget(ui->label_frame);

    videoWidget->setAttribute(Qt::WA_DeleteOnClose);
    player->setVideoOutput(videoWidget);

    data_folder = "C:/Users/ephra/Documents/Capstone/SparePartsSaddleyeCapstone/learning/";
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_Done_Button_released()
{
    QVideoFrame videoframe;
    bool isTrail = false;
    std::vector<std::string> categories_type;
    std::vector<std::string> categories_condition;

    //Check if trail or not
    if(ui->TrailCheck->checkState())
    {
        isTrail = true;
    }

    //Check all the states possible under trail type
    if(ui->AsphaltCheck->checkState())
    {
        categories_type.push_back("Asphalt");
    }

    if(ui->DirtCheck->checkState())
    {
        categories_type.push_back("Dirt");
    }

    if(ui->GravelCheck->checkState())
    {
        categories_type.push_back("Gravel");
    }

    if(ui->SidewalkCheck->checkState())
    {
        categories_type.push_back("Sidewalk");
    }

    //Check all possible conditions of the trail
    if(ui->WetCheck->checkState())
    {
        categories_condition.push_back("Wet");
    }

    if(ui->DryCheck->checkState())
    {
        categories_condition.push_back("Dry");
    }

    copy_files(isTrail, categories_type, categories_condition, videoframe);
}

void MainWindow::copy_files(bool isTrail, std::vector<std::string> categories_type, std::vector<std::string> categories_condition, QVideoFrame videoframe)
{
    std::string to_path;
    std::string from_path = "C:/Users/ephra/Pictures/This folder is cursed/Cory using is special laser vision glasses just like cyclops from the xmen.png";
    std::string file_name = "TestFile.png";

    if(isTrail)
    {
        to_path = data_folder + "data_trail/test/Trail/" + file_name;
    }
    else
    {
        to_path = data_folder + "data_trail/test/Not_trail/" + file_name;
    }

    std::ofstream(to_path, std::ios::binary) << videoframe.bits();
    //std::ofstream(to_path, std::ios::binary) << std::ifstream(from_path, std::ios::binary).rdbuf();

    for(int i = 0; i < categories_type.size(); i++)
    {
         to_path = data_folder + "data/test/" + categories_type[i] + "/" + file_name;
         std::ofstream(to_path, std::ios::binary) << std::ifstream(from_path, std::ios::binary).rdbuf();
    }

    for(int i = 0; i < categories_condition.size(); i++)
    {
         to_path = data_folder + "data_moist/test/" + categories_condition[i] + "/" + file_name;
         std::ofstream(to_path, std::ios::binary) << std::ifstream(from_path, std::ios::binary).rdbuf();
    }
}

void MainWindow::on_BullshitButton_released()
{
    player->setMedia(QUrl::fromLocalFile("C:/Users/ephra/Videos/Captures/The problematic queen.mp4"));
    videoWidget->setGeometry(0,0,1280,720);
    videoWidget->show();
    player->play();

    /*
    QMediaPlayer *player = new QMediaPlayer();
    QVideoProbe *probe = new QVideoProbe;

    connect(probe, SIGNAL(videoFrameProbed(QVideoFrame)), this, SLOT(processFrame(QVideoFrame)));

    probe->setSource(player); // Returns true, hopefully.

    player->setVideoOutput(myVideoSurface);
    player->setMedia(QUrl::fromLocalFile("observation.mp4"));
    player->play(); // Start receiving frames as they get presented to myVideoSurface
*/}
