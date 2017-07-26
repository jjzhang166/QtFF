#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QThread>
#include "myffmpeg.h"
class VideoPlayer:public QThread
{
    Q_OBJECT
public:
    VideoPlayer();

protected:
    void run();


};

#endif // VIDEOPLAYER_H
