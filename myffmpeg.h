#ifndef MYFFMPEG_H
#define MYFFMPEG_H

#include <string>
#include <iostream>
using namespace std;

#include"cavframe.h"
#include"cavpacket.h"

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
    bool OpenUrl();
    void setFilename(string filename);
    string getFilename() const;
    bool CloseUrl();
    void avDumpFormat();  //打印出视频的信息
    void run();
    int avReadFrame(CAVPacket cpacket);

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
    bool running = false;
    bool isPause = false;
signals:
    void sig_GetOneFrame(QImage img);
public slots:

    void play();
    void stop();
};

#endif // MYFFMPEG_H
