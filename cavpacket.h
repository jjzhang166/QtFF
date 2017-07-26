#ifndef CAVPACKET_H
#define CAVPACKET_H

extern "C"
{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libavutil/pixfmt.h"
    #include "libswscale/swscale.h"
}
class CAVPacket
{
public:
    CAVPacket(int size);
    ~CAVPacket();
    AVPacket * getAVPacket();
private:
    AVPacket packet;

};

#endif // CAVPACKET_H
