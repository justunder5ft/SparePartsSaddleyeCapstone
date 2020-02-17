#include "mainwindow.h"
#include "framethreader.h"
#include "common.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    FrameThreader processing_thread;
    processing_thread.name = "FrameProcessor";
    global_processing_thread = &processing_thread;

    processing_thread.start();

    w.show();
    return a.exec();
}
