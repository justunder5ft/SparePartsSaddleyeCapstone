#ifndef FRAMETHREADER_H
#define FRAMETHREADER_H
#include "ui_mainwindow.h"
#include <QThread>
#include <QVideoFrame>
#include <QMediaPlayer>
#include <queue>

class FrameThreader : public QThread
{
public:
    FrameThreader();
    void setValues(int new_file_num, QMediaPlayer* new_player, Ui::MainWindow* new_ui, QString new_data_folder);
    void run();
    void write(QString to_path, QByteArray ba);
    void copy_files(bool isTrail, std::vector<QString> categories_type, std::vector<QString> categories_condition);
    void processFrame();

    int file_num = 0;
    QVideoFrame videoframe;
    QMediaPlayer* player;
    Ui::MainWindow* ui;
    QString data_folder;
    QString name;
    std::queue<QVideoFrame> frame_queue;

    bool status_process = false;
    bool status_kill = false;
};

#endif // FRAMETHREADER_H
