#include "framethreader.h"
#include <QMediaPlayer>
#include <QFile>
#include <QDir>
#include <QVideoFrame>
#include <QBuffer>
#include "common.h"
#include "ui_mainwindow.h"

FrameThreader::FrameThreader()
{
    return;
}

void FrameThreader::setValues(int new_file_num, QMediaPlayer* new_player, Ui::MainWindow* new_ui, QString new_data_folder)
{
    file_num = new_file_num;
    player = new_player;
    ui = new_ui;
    data_folder = new_data_folder;

    return;
}

void FrameThreader::run()
{
    while(1)
    {
        //If you need to process
        if(this->status_process)
        {
            processFrame();
        }

        //exit signal
        if(this->status_kill)
        {
            break;
        }
    }
}

void FrameThreader::copy_files(bool isTrail, std::vector<QString> categories_type)
{
    QString to_path;
    QString real_file_name;
    QString file_name = video_name;

    QDir dir ;

    real_file_name = file_name + QString::number(file_num++) + ".jpeg";


    //qDebug() << "DataFolder : " <<data_folder;
    //Write to trail folder if trail box is checked, write to not trail folder if not
    if(isTrail)
    {
        to_path = data_folder + "/Trail" ;
        if(!dir.exists(to_path))
        {
            dir.mkpath(to_path);
        }
        to_path = data_folder + "/Trail/" + real_file_name;
    }
    else
    {
        to_path = data_folder + "/Not_trail" ;
        if(!dir.exists(to_path))
        {
            dir.mkpath(to_path);
        }
        to_path = data_folder + "/Not_trail/" + real_file_name;
    }

    //Convert into a Qimage format
    QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(videoframe.data.pixelFormat());
    QImage frame_image(videoframe.data.bits(), videoframe.data.width(), videoframe.data.height(), videoframe.data.bytesPerLine(), imageFormat);
    frame_image = frame_image.mirrored(false, true); //Flip horizontal to have image upright

    //Convert to buffer to save image to machine
    QByteArray ba;
    QBuffer buffer;
    buffer.buffer().resize(1000000);
    buffer.setBuffer(&ba);

    //Open buffer for writing
    buffer.open(QIODevice::ReadWrite);
    frame_image.save(&buffer, "JPEG");
    write(to_path, ba); //Write if trail or not trail

    //Write data for trail type categories (Asphalt, Sidewalk, etc.)
    for(int i = 0; i < categories_type.size(); i++)
    {
        to_path = data_folder + "/" + categories_type[i] ;
        if(!dir.exists(to_path))
        {
            dir.mkpath(to_path);
        }
         to_path = data_folder + "/" + categories_type[i] + "/" + real_file_name;
         write(to_path, ba);
    }

    //Update progress
    global_total_processed_frames++;
    ui->FrameProgressLabel->setText("Frames Processed: " + QString::number(global_total_processed_frames) + " of " + QString::number(global_total_enqueued_frames));
    return;
}

//This is called constantly within Run and long as the thread is active
void FrameThreader::processFrame()
{
    if(frame_queue.empty())
    {
        return;
    }

    //Get the next frame in the queue
    videoframe = frame_queue.front();
    frame_queue.pop();
    frame_count++;

    bool isTrail = false;
    std::vector<QString> categories_type;
    videoframe.data.map(QAbstractVideoBuffer::ReadOnly); //Allows the data of the frame to be read and converted to an image format

    //Check if trail or not
    if(videoframe.trail)
    {
        isTrail = true;
    }

    //Check all the states possible under trail type
    if(videoframe.asphalt)
    {
        categories_type.push_back("Asphalt");
    }

    if(videoframe.dirt)
    {
        categories_type.push_back("Dirt");
    }

    if(videoframe.gravel)
    {
        categories_type.push_back("Gravel");
    }

    if(videoframe.sidewalk)
    {
        categories_type.push_back("Sidewalk");
    }

    if(videoframe.custom)
    {
        if(videoframe.custom_name.length() > 0) //Only process if non-empty
        {
            categories_type.push_back(videoframe.custom_name);
        }
    }

    //Check all possible conditions of the trail
    if(videoframe.wet)
    {
        categories_type.push_back("Wet");
    }

    if(videoframe.dry)
    {
        categories_type.push_back("Dry");
    }

    //If this frame should not be skipped, then save the image to specified locations
    if(frame_count%ui->FrameStepper->value() == 0)
    {
        copy_files(isTrail, categories_type);
        frame_count = 0;
    }

    return;
}

void FrameThreader::UpdateDataFolder(QString new_data_folder)
{
    data_folder = new_data_folder; // set the new data folder
}

//Write data to actual file locations
void FrameThreader::write(QString to_path, QByteArray ba)
{
    QFile file_location(to_path);

    file_location.open(QIODevice::ReadWrite);
    file_location.write(ba);
    file_location.close();
}

void FrameThreader::SetVideoName(QString videoPath){
    QStringList VideoDelimList = videoPath.split('/');
    QString Temp = VideoDelimList.last(); // get rid of the path

    QStringList VideoNameList = Temp.split('.'); // get rid of the extention.
    video_name = VideoNameList.first(); // save the name

   return ;
}
