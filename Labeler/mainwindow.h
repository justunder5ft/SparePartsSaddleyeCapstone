#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVideoFrame>
#include <QAbstractVideoSurface>
#include <string>
#include <vector>
#include <QVideoWidget>
#include <QMediaPlayer>
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
    QVideoWidget* videoWidget;

private slots:
    void on_Done_Button_released();

    void on_BullshitButton_released();

private:
    Ui::MainWindow *ui;
    void copy_files(bool isTrail, std::vector<std::string> categories, std::vector<std::string> categories_condition, QVideoFrame videoframe);
};
#endif // MAINWINDOW_H
