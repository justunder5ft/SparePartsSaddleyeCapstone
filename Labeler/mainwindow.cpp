#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "framethreader.h"
#include "framedata.h"
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

    //Initialize video processing elements
    player = new QMediaPlayer;
    videoWidget = new QVideoWidget(ui->label_frame);
    frame_probe = new QVideoProbe;

    //Connects slider to video
    m_slider = (ui->PlaybackSlider);
    connect(m_slider, &QSlider::sliderMoved, this, &MainWindow::seek);
    m_slider->setRange(0, player->duration() / 1000);

    //Connect media player to ui element
    player->setVideoOutput(videoWidget);

    //Set slider to update with video / vice versa
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::durationChanged);
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::positionChanged);

    //Path where data frames are stored
    data_folder = QDir::currentPath();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Select video and prepares it for play (hard coded to video in my local system at the moment)
void MainWindow::on_process_button_released()
{
    file_num = 0; //Resent file number to 0
    global_processing_thread->status_process = true; //Start the thread (only really matters the first time play is hit)
    global_processing_thread->setValues(file_num, player, ui, data_folder); //Set values based on current loaded video

    //Set up frame prober (This is what actually grabs individual frames)
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
    while(!global_processing_thread->frame_queue.empty())
    {
        global_processing_thread->frame_queue.pop();
    }
}

/* Code taken from Qt's media player example */
void MainWindow::durationChanged(qint64 duration)
{
    m_duration = duration / 1000;
    m_slider->setMaximum(m_duration);
}
//Updates video slider position
void MainWindow::positionChanged(qint64 progress)
{
    if (!m_slider->isSliderDown())
        m_slider->setValue(progress / 1000);

    updateDurationInfo(progress / 1000);

       //qDebug() << "Player Position" << player->position();
}

//Allows user to change slider position and change video state
void MainWindow::seek(int seconds)
{
    player->setPosition(seconds * 1000);
}

//Updates current time stamp of the video (Not used in our app but could be useful)
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

//Process by frame and save frame to appropriate locations (This is done for EVERY individual frame in the video
void MainWindow::processFrame(QVideoFrame the_frame) {
        framedata temp; //New frame data object
        temp.data = the_frame; //QVideoFrame object (Holds frame data)
        temp.asphalt = ui->AsphaltCheck->checkState(); //Booleans for check boxes
        temp.gravel = ui->GravelCheck->checkState();
        temp.dirt = ui->DirtCheck->checkState();
        temp.sidewalk = ui->SidewalkCheck->checkState();
        temp.trail = ui->TrailCheck->checkState();
        temp.dry = ui->DryCheck->checkState();
        temp.wet = ui->WetCheck->checkState();
        temp.custom = ui->CustomCheck->checkState();
        temp.custom_name = ui->CustomFolderTextBox->text(); //Text of custom check box
        global_processing_thread->frame_queue.push(temp); // push it to the queue for processing
        global_total_enqueued_frames++;
}

//Desctructor for mainwindow, kills thread then exits
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

//Function that opens discovery window to select video file (Only searched for .mp4s but can be modified.
void MainWindow::on_file_select_button_released()
{
    QString path = qApp->applicationDirPath();
    QDir dir;

    //Add more formats to this function to add more formats
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

//Function that opens discovery window to save location.
void MainWindow::on_data_folder_button_released()
{
    QString path = qApp->applicationDirPath();
    QDir dir;

    QString newDataFolderName = QFileDialog::getExistingDirectory(this,
            "Choose location of data folder", path);

    if(dir.exists(path) && path.length() != 0)
    {
            data_folder = newDataFolderName;
            global_processing_thread->UpdateDataFolder(newDataFolderName);
    }
}


