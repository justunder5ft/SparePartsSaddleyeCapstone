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
#include <QSlider>
#include <QTime>
#include <QVideoSurfaceFormat>
#include <QRect>
#include <QVideoRendererControl>

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
    frame_probe = new QVideoProbe;

    m_slider = (ui->PlaybackSlider);
    connect(m_slider, &QSlider::sliderMoved, this, &MainWindow::seek);
    m_slider->setRange(0, player->duration() / 1000);

    player->setVideoOutput(videoWidget);

    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::durationChanged);
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::positionChanged);

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

    videoframe = checkedFrame;

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

    isButtonClicked = true;
    QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(videoframe.pixelFormat());
    QImage frame_image(videoframe.bits(), videoframe.width(), videoframe.height(), videoframe.bytesPerLine(), imageFormat);

    //frame_image.save(to_path.c_str(), 0);
    /*THIS IS WHERE THE IMAGE WILL BE COPIED*/
    for(int i = 0; i < frame_image.bitPlaneCount(); i++)
    {
        int BPL = frame_image.bytesPerLine();

        std::ofstream(to_path, std::ios::binary) << *(frame_image.bits() + i);
    }
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
    if(frame_probe->setSource(player))
    {
        connect(frame_probe, &QVideoProbe::videoFrameProbed, this, &MainWindow::processFrame);// Returns true, hopefully.
    }

    player->setMedia(QUrl::fromLocalFile("C:/Users/ephra/Videos/Captures/The problematic queen.mp4"));
    //Set format of video
    video_format.setFrameRate(frame_rate);
    video_format.setFrameSize(1280,720);
    video_format.setPixelAspectRatio(16,9);
    //video_format.setProperty("Video", 1);
    //QVideoRendererControl *rendererControl = QMediaService::player->requestControl<QVideoRendererControl *>();
    //rendererControl->setSurface(videoWidget);
    videoWidget->setGeometry(0,0,1280,720);
    //videoWidget->start(video_format);
    videoWidget->show();
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

void MainWindow::processFrame(QVideoFrame the_frame) {
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

  copy_files(isTrail, categories_type, categories_condition, the_frame);
  qDebug() << "What";

  return;
}
