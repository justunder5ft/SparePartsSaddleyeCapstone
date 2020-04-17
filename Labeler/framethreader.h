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
    void copy_files(bool isTrail, std::vector<QString> categories_type);
    void processFrame();
    void UpdateDataFolder(QString new_data_folder);

    int file_num = 0;
    int frame_skip = 1; // amount of frames to skip before queueing
    int frame_count = 0; // current frame count
    QString video_name = "NO_VID";
    QVideoFrame videoframe;
    QMediaPlayer* player;
    Ui::MainWindow* ui;
    QString data_folder;
    QString name;
    std::queue<QVideoFrame> frame_queue;
    void SetVideoName(QString videoPath);

    bool status_process = false;
    bool status_kill = false;
private slots:
    void on_FrameStepper_valueChanged(int arg1);
    void on_play_button_pressed();
    void on_BullshitButton_released();
    void on_process_button_released();
    void on_frame_progress_box_textChanged();
};

#endif // FRAMETHREADER_H
