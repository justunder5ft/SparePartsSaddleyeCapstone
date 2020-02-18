#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVideoFrame>
#include <QAbstractVideoSurface>
#include <string>
#include <vector>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QSlider>
#include <QVideoProbe>
#include <QVideoSurfaceFormat>
#include <QBuffer>
#include <QCloseEvent>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString data_folder;
    QMediaPlayer* player; //Responsible for actually playing the video
    QVideoWidget* videoWidget; //Responsible for housing the video in UI
    QVideoProbe* frame_probe; //Responsible for grabbing frames
    QSlider* m_slider; //Responsible for controlling the video
    qint64 m_duration;
    qreal playback_rate = 1;
    void processFrame(QVideoFrame the_frame); //Grab a given frame
    void durationChanged(qint64 duration); //Update slider
    void positionChanged(qint64 progress); //Update slider
    void updateDurationInfo(qint64 currentInfo); //Update slider
    void write(std::string to_path, QByteArray ba); //Write file to a folder
    void closeEvent (QCloseEvent *event);

    QVideoFrame checkedFrame;
    int file_num = 0;
    QString video_file_source = "C:/Users/ephra/Videos/Captures/Arcana.mp4";

private slots:
    void on_Done_Button_released();
    void seek(int seconds);
    void on_BullshitButton_released();
    void on_play_button_released();
    void on_pause_button_released();
    void on_file_select_button_released();
    void on_data_folder_button_released();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
