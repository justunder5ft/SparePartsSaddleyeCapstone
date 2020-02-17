#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "framethreader.h"
#include "common.h"
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
#include <QSlider>
#include <QTime>
#include <QVideoSurfaceFormat>
#include <QRect>
#include <QVideoRendererControl>
#include <QBuffer>
#include <QSaveFile>
#include <QThread>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //This is cory, this won't work on your machine, and really you don't need it
    std::string file_location = "C:/Users/ephra/Pictures/This folder is cursed/Cory using is special laser vision glasses just like cyclops from the xmen";
    ui->setupUi(this);
    QPixmap video_frame(file_location.c_str());
    ui->label_frame->setPixmap(video_frame.scaled(1280, 720, Qt::KeepAspectRatioByExpanding));
    //End cory set up

    //Initialize video processing elements
    player = new QMediaPlayer;
    videoWidget = new QVideoWidget(ui->label_frame);
    frame_probe = new QVideoProbe;

    //Connects slider to video (Will not work until i fix the buffer making everything slow)
    m_slider = (ui->PlaybackSlider);
    connect(m_slider, &QSlider::sliderMoved, this, &MainWindow::seek);
    m_slider->setRange(0, player->duration() / 1000);

    //Connect media player to ui element
    player->setVideoOutput(videoWidget);

    //Set slider to update with video / vice versa
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::durationChanged);
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::positionChanged);

    //Path where data frames are stored, edit as needed until i modularize
    data_folder = "C:/Users/ephra/Documents/Capstone/SparePartsSaddleyeCapstone/learning/";
    qDebug() << global_processing_thread;
}

MainWindow::~MainWindow()
{
    delete ui;
}

//This function is not being used right now, it just copies cory to a folder.
void MainWindow::on_Done_Button_released()
{
    /*
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

    videoframe = checkedFrame;

    copy_files(isTrail, categories_type, categories_condition, videoframe);
*/}

//Writes files to data folder per frame
void MainWindow::copy_files(bool isTrail, std::vector<std::string> categories_type, std::vector<std::string> categories_condition, QVideoFrame videoframe)
{
    std::string to_path;
    std::string real_file_name;
    std::string from_path = "C:/Users/ephra/Pictures/This folder is cursed/Cory using is special laser vision glasses just like cyclops from the xmen.png";
    std::string file_name = "TestFile";

    real_file_name = file_name + std::to_string(file_num) + ".png";

    //Write to trail folder if trail box is checked, write to not trail folder if not
    if(isTrail)
    {
        to_path = data_folder + "data_trail/test/Trail/" + real_file_name;
    }
    else
    {
        to_path = data_folder + "data_trail/test/Not_trail/" + real_file_name;
    }

    QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(videoframe.pixelFormat());
    QImage frame_image(videoframe.bits(), videoframe.width(), videoframe.height(), videoframe.bytesPerLine(), imageFormat);
    frame_image = frame_image.mirrored(false, true);

    QByteArray ba;
    QBuffer buffer;
    buffer.buffer().resize(1000000);
    buffer.setBuffer(&ba);

    buffer.open(QIODevice::ReadWrite);
    frame_image.save(&buffer, "PNG");
    write(to_path, ba); //Write if trail or not trail

    qDebug() << ba.size();

    //Write data for trail type categories (Asphalt, Sidewalk, etc.)
    for(int i = 0; i < categories_type.size(); i++)
    {
         to_path = data_folder + "data/test/" + categories_type[i] + "/" + real_file_name;
         write(to_path, ba);
    }

    //Write data for trail condition categories (Wet, Dry, Etc.)
    for(int i = 0; i < categories_condition.size(); i++)
    {
         to_path = data_folder + "data_moist/test/" + categories_condition[i] + "/" + real_file_name;
         //std::ofstream(to_path, std::ios::binary) << std::ifstream(from_path, std::ios::binary).rdbuf();
         write(to_path, ba);
    }
}

//(Will change name eventually) Select video and prepares it for play (hard coded to video in my local system at the moment)
void MainWindow::on_BullshitButton_released()
{
    global_processing_thread->status_process = true;
    global_processing_thread->setValues(file_num, player, ui, data_folder);


    //Set up frame prober
    if(frame_probe->setSource(player))
    {
        connect(frame_probe, &QVideoProbe::videoFrameProbed, this, &MainWindow::processFrame);// Returns true, hopefully.
    }

    //Video file on my system, change as needed
    player->setMedia(QUrl::fromLocalFile("C:/Users/ephra/Videos/Captures/The problematic queen.mp4"));

    //Display video
    videoWidget->setGeometry(0,0,1280,720);
    player->setPlaybackRate(4);
    player->setMuted(true);
    //videoWidget->show();
    player->play();
}

/* Code taken from Qt's media player example */

void MainWindow::durationChanged(qint64 duration)
{
    m_duration = duration / 1000;
    m_slider->setMaximum(m_duration);
}

void MainWindow::positionChanged(qint64 progress)
{
    if (!m_slider->isSliderDown())
        m_slider->setValue(progress / 1000);

    updateDurationInfo(progress / 1000);
}

void MainWindow::seek(int seconds)
{
    player->setPosition(seconds * 1000);
}

void MainWindow::updateDurationInfo(qint64 currentInfo)
{
    QString tStr;
    if (currentInfo || m_duration) {
        QTime currentTime((currentInfo / 3600) % 60, (currentInfo / 60) % 60,
            currentInfo % 60, (currentInfo * 1000) % 1000);
        QTime totalTime((m_duration / 3600) % 60, (m_duration / 60) % 60,
            m_duration % 60, (m_duration * 1000) % 1000);
        QString format = "mm:ss";
        if (m_duration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    //m_labelDuration->setText(tStr);
}

//Process by frame and save frame to appropriate locations
void MainWindow::processFrame(QVideoFrame the_frame) {

    global_processing_thread->frame_queue.push(the_frame);
/*
    //Otherwise, send the frame wherever.
    bool isTrail = false;
    std::vector<std::string> categories_type;
    std::vector<std::string> categories_condition;
    the_frame.map(QAbstractVideoBuffer::ReadOnly);
*/
    //Thread here
    //FrameThreader new_thread(file_num++, the_frame, player, ui, data_folder);
    //new_thread.start();

    return;
/*
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

    copy_files(isTrail, categories_type, categories_condition, the_frame);

    return;
*/}

//Write data to actual file locations
void MainWindow::write(std::string to_path, QByteArray ba)
{
    QFile file_location(to_path.c_str());

    file_location.open(QIODevice::ReadWrite);
    file_location.write(ba);
    file_location.close();
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "MIDI_MIDI",
                                                                tr("Are you sure?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    qDebug()<< global_processing_thread;
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        global_processing_thread->status_kill = true;
        global_processing_thread->quit();
        while(global_processing_thread->isRunning()) // wait for thread to kill itself
        {
            //do nothing
        }
        // then you may close the program
        event->accept();
    }
}
