#ifndef FRAMETHREADER_H
#define FRAMETHREADER_H
#include "ui_mainwindow.h"
#include "framedata.h"
#include <QThread>
#include <QVideoFrame>
#include <QMediaPlayer>
#include <queue>
/*
 * Framethreader is responsible for processing and saving the frames as JPEGs within the frame
 * queue. As long as the queue that holds "framedata" objects is not empty, frames will be processed based on
 * the skip amount specified in the main window.
 */
class FrameThreader : public QThread
{
public:
    FrameThreader();
    void setValues(int new_file_num, QMediaPlayer* new_player, Ui::MainWindow* new_ui, QString new_data_folder);
    void run(); //Thread processing, always running
    void write(QString to_path, QByteArray ba); //Function that actually writes the files based on data gathered
    void copy_files(bool isTrail, std::vector<QString> categories_type); //Function that determines all file locations and converts format to jpeg
    void processFrame(); //Obtains frame data from queue and checks state of boolean values for the frame
    void UpdateDataFolder(QString new_data_folder); //Update file save location
    void SetVideoName(QString videoPath); //Updates name of jpeg file based on video's name

    int file_num = 0;
    int frame_skip = 1; // amount of frames to skip before queueing
    int frame_count = 0; // current frame count
    QString video_name = "NO_VID";
    framedata videoframe;
    QMediaPlayer* player;
    Ui::MainWindow* ui;
    QString data_folder;
    QString name;
    std::queue<framedata> frame_queue;

    bool status_process = false;
    bool status_kill = false;
};

#endif // FRAMETHREADER_H
