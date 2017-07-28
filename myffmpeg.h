#ifndef MYFFMPEG_H
#define MYFFMPEG_H

#include <string>
#include <list>

#include <iostream>
using namespace std;

#include"cavframe.h"
#include"cavpacket.h"

#include "SDL.h"


#include <QObject>
#include <QImage>

#include <QThread>
extern "C"
{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libavutil/pixfmt.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"

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
    int initSDL();
private:
    AVFormatContext *pFormatCtx;
    AVCodecContext * pCodecCtx;
    AVCodec *pCodec;

    AVCodecContext * pCodecCtx_A;
    AVCodec* pCodec_A;
    AVFrame *pFrame, *pFrameRGB;
    AVPacket *packet;

    struct SwsContext *img_convert_ctx;
    int videoStream;
    int audioStream;
    int dataStream;
    string filename;
    bool running = true;
    bool isplay = true;
    SDL_AudioSpec wanted_spec;

   // void fill_audiodata(void * udata,uint8_t * stream,int len);

signals:
    void sig_GetOneFrame(QImage img);
public slots:

    void play();
    void stop();
};

#endif // MYFFMPEG_H
