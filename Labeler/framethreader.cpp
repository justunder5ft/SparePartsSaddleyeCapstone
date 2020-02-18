#include "framethreader.h"
#include <QMediaPlayer>
#include <QFile>
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

void FrameThreader::copy_files(bool isTrail, std::vector<QString> categories_type, std::vector<QString> categories_condition)
{
    QString to_path;
    QString real_file_name;
    QString from_path = "C:/Users/ephra/Pictures/This folder is cursed/Cory using is special laser vision glasses just like cyclops from the xmen.png";
    QString file_name = "TestFile";

    real_file_name = file_name + QString::number(file_num++) + ".png";

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

void FrameThreader::processFrame()
{
    if(frame_queue.empty())
    {
        return;
    }

    videoframe = frame_queue.front();
    frame_queue.pop();

    bool isTrail = false;
    std::vector<QString> categories_type;
    std::vector<QString> categories_condition;
    videoframe.map(QAbstractVideoBuffer::ReadOnly);

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

    copy_files(isTrail, categories_type, categories_condition);

    return;
}

//Write data to actual file locations
void FrameThreader::write(QString to_path, QByteArray ba)
{
    QFile file_location(to_path);

    file_location.open(QIODevice::ReadWrite);
    file_location.write(ba);
    file_location.close();
}
