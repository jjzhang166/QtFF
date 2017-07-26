#ifndef CAVFRAME_H
#define CAVFRAME_H


extern "C"
{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libavutil/pixfmt.h"
    #include "libswscale/swscale.h"
}


class CAVFrame
{
public:
    CAVFrame();
    ~CAVFrame();
private:
    AVFrame* pAVFrame;
};

#endif // CAVFRAME_H
