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
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    std::string data_folder;
    QMediaPlayer* player;
    //QMediaService* player;
    //FrameGrabber* videoWidget;
    QVideoWidget* videoWidget;
    QVideoProbe* frame_probe;
    QSlider* m_slider;
    QVideoSurfaceFormat video_format;
    QSize aspect_ratio;
    qint64 m_duration;
    qreal frame_rate = 60;
    void processFrame(QVideoFrame the_frame);
    void durationChanged(qint64 duration);
    void positionChanged(qint64 progress);
    void updateDurationInfo(qint64 currentInfo);
    bool isButtonClicked = false;
    QVideoFrame checkedFrame;


private slots:
    void on_Done_Button_released();
    void seek(int seconds);
    void on_BullshitButton_released();

private:
    Ui::MainWindow *ui;
    void copy_files(bool isTrail, std::vector<std::string> categories, std::vector<std::string> categories_condition, QVideoFrame videoframe);
};
#endif // MAINWINDOW_H
