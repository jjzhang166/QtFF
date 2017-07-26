#include "cavpacket.h"

CAVPacket::CAVPacket(int size)
{
    av_new_packet(&packet,size);
}
CAVPacket::~CAVPacket()
{
    av_free_packet(&packet);
}

AVPacket *CAVPacket::getAVPacket()
{
    return &packet;
}
