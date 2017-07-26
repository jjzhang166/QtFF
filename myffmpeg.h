#ifndef MYFFMPEG_H
#define MYFFMPEG_H

#include <string>
#include <iostream>
using namespace std;



#include <QObject>
#include <QImage>

#include <QThread>
extern "C"
{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libavutil/pixfmt.h"
    #include "libswscale/swscale.h"
}


class MyFFmpeg:public QThread
{
    Q_OBJECT
public:
    MyFFmpeg();
    ~MyFFmpeg();
    bool OpenUrl(string filepath);
    void setFilename(string filename);
    bool CloseUrl();
    void run();
private:
    AVFormatContext *pFormatCtx;
    AVCodecContext * pCodecCtx;
    AVCodec *pCodec;
    AVFrame *pFrame, *pFrameRGB;
    AVPacket *packet;
    uint8_t * out_buffer;
    struct SwsContext *img_convert_ctx;
    int videoStream;
    int audioStream;
    int dataStream;
    string filename;
signals:
    void sig_GetOneFrame(QImage img);
};

#endif // MYFFMPEG_H