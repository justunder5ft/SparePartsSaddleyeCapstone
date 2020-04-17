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
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //This is cory, this won't work on your machine, and really you don't need it(Just change this to any image file path)
    //Or comment out this section
    std::string file_location = "C:/Users/tcarr/Pictures/Capture.png";
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
    //frameCount=0;

    //Connect media player to ui element
    player->setVideoOutput(videoWidget);

    //Set slider to update with video / vice versa
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::durationChanged);
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::positionChanged);

    //Path where data frames are stored, edit as needed until i modularize
    data_folder = QDir::currentPath();
    qDebug() << QDir::currentPath();
    qDebug() << global_processing_thread;
}

MainWindow::~MainWindow()
{
    delete ui;
}

//(Will change name eventually) Select video and prepares it for play (hard coded to video in my local system at the moment)
void MainWindow::on_process_button_released()
{
    file_num = 0;
    global_processing_thread->status_process = true;
    global_processing_thread->setValues(file_num, player, ui, data_folder);

    //Set up frame prober
    if(frame_probe->setSource(player))
    {
        connect(frame_probe, &QVideoProbe::videoFrameProbed, this, &MainWindow::processFrame);// Returns true, hopefully.
    }

    //Video file on my system, change as needed
    player->setMedia(QUrl::fromLocalFile(video_file_source));

    //Display video
    videoWidget->setGeometry(0,0,1280,720);
    player->setPlaybackRate(playback_rate);
    player->setMuted(true);
    global_total_enqueued_frames = 0;
    global_total_processed_frames = 0;
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

       //qDebug() << "Player Position" << player->position();
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
        global_processing_thread->frame_queue.push(the_frame); // push it to the queue for processing
        global_total_enqueued_frames++;
}

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
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "Labeler",
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

void MainWindow::on_play_button_released()
{
    player->play();
}

void MainWindow::on_pause_button_released()
{
    player->pause();
}

void MainWindow::on_file_select_button_released()
{
    QString path = qApp->applicationDirPath();
    QDir dir;

    QString fileName = QFileDialog::getOpenFileName(this,
            "Choose video file", path,
            "Video (*.mp4)");
    QFile file(fileName);

    if(dir.exists(path))
    {
        if (file.open(QIODevice::ReadOnly))
        {
            video_file_source = fileName;
            global_processing_thread->SetVideoName(fileName);// set the videoName in the thread for naming conventions.
        }

    }

    file.close();
}


void MainWindow::on_data_folder_button_released()
{
    QString path = qApp->applicationDirPath();
    QDir dir;

    QString newDataFolderName = QFileDialog::getExistingDirectory(this,
            "Choose location of data folder", path);

    if(dir.exists(path))
    {
            data_folder = newDataFolderName;
            global_processing_thread->UpdateDataFolder(newDataFolderName);
    }
}


