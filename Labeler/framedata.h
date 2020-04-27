#ifndef FRAMEDATA_H
#define FRAMEDATA_H
#include <QVideoFrame>
#include <QString>

/*
 *  The framedata class was created to hold both the frames' raw data and the checkbox
 *  states of the app at the time the frame is processed. Each bool corresponds to a checkbox in main
 *  window and the custom name is the info in the custom box name.
 */
class framedata
{
public:
    framedata();
    QVideoFrame data;
    bool gravel;
    bool asphalt;
    bool dirt;
    bool sidewalk;
    bool trail;
    bool dry;
    bool wet;
    bool custom;
    QString custom_name;
};

#endif // FRAMEDATA_H
