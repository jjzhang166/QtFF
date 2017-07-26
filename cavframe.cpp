#include "cavframe.h"

CAVFrame::CAVFrame()
{
    pAVFrame  = av_frame_alloc();
}

CAVFrame::~CAVFrame()
{
    av_frame_free(&pAVFrame);
}
