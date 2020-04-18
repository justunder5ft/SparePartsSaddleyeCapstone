#ifndef FRAMEDATA_H
#define FRAMEDATA_H
#include <QVideoFrame>
#include <QString>

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
