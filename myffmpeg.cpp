#include "myffmpeg.h"

MyFFmpeg::MyFFmpeg()
{
    cout<<__FUNCTION__<<__LINE__<<endl;
    av_register_all();
    avformat_network_init();
}

MyFFmpeg::~MyFFmpeg()
{
    avformat_free_context(pFormatCtx);
    avformat_network_deinit();
}

bool MyFFmpeg::OpenUrl()
{
 //    cout<<__FUNCTION__<<__LINE__<<endl;
    //this->filename = filepath;
    pFormatCtx = avformat_alloc_context();
    cout<<"the file is:"<<filename.c_str()<<endl;
    if (avformat_open_input(&pFormatCtx, filename.c_str(), NULL, NULL) != 0) {
        printf("can't open the file. \n");
        return false;
    }
   //  cout<<__FUNCTION__<<__LINE__<<endl;
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        printf("Could't find stream infomation.\n");
        return false;
    }
  //  cout<<__FUNCTION__<<__LINE__<<endl;
    videoStream = -1;
    audioStream = -1;
    dataStream = -1;
    for (int i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
        }
        else if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            audioStream = i;
        }
        else if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_DATA)
        {
            dataStream = i;
        }

    }
    if (videoStream == -1) {
        printf("Didn't find a video stream.\n");
    }
    if (audioStream == -1) {
        printf("Didn't find a audio Stream.\n");
    }
    if (dataStream == -1) {
        printf("Didn't find a data Stream.\n");
    }

    ///查找解码器
    pCodecCtx = pFormatCtx->streams[videoStream]->codec;
    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
 cout<<__FUNCTION__<<__LINE__<<endl;
    if (pCodec == NULL) {
        printf("Codec not found.\n");
        return false;
    }

    ///打开解码器
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        printf("Could not open codec.\n");
        return false;
    }

    pFrame = av_frame_alloc();
    pFrameRGB = av_frame_alloc();

    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
            pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height,
            AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);

    int numBytes = avpicture_get_size(AV_PIX_FMT_RGB32, pCodecCtx->width,pCodecCtx->height);

    out_buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    avpicture_fill((AVPicture *) pFrameRGB, out_buffer, AV_PIX_FMT_RGB32,
            pCodecCtx->width, pCodecCtx->height);

    int y_size = pCodecCtx->width * pCodecCtx->height;

    packet = (AVPacket *) malloc(sizeof(AVPacket)); //分配一个packet
    av_new_packet(packet, y_size); //分配packet的数据

    av_dump_format(pFormatCtx, 0, filename.c_str(), 0); //输出视频信息

    int got_picture;
    //cout<<__FUNCTION__<<__LINE__<<endl;
    while (running)
    {
        if (av_read_frame(pFormatCtx, packet) < 0)
        {
            break; //这里认为视频读取完了
        }
        while(!isplay){
            msleep(20);
            if(!running)
                break;
        }

        if (packet->stream_index == videoStream) {
            int ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture,packet);

            if (ret < 0) {
                printf("decode error.\n");
                return false;
            }

            if (got_picture) {
                sws_scale(img_convert_ctx,
                        (uint8_t const * const *) pFrame->data,
                        pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data,
                        pFrameRGB->linesize);

                //cout<<__FUNCTION__<<__LINE__<<endl;
                //SaveFrame(pFrameRGB, pCodecCtx->width,pCodecCtx->height,index++);   //保存图片
                //cout<<__FUNCTION__<<__LINE__<<endl;
                QImage tmpImg ((uchar*) out_buffer,pCodecCtx->width,pCodecCtx->height,QImage::Format_RGB32);
                QImage image = tmpImg.copy();
                emit sig_GetOneFrame(image);
                //cout<<__FUNCTION__<<__LINE__<<endl;
            }
        }
        av_free_packet(packet);
        msleep(10); //停一停  不然放的太快了
    }
    av_free(out_buffer);
    av_free(pFrameRGB);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);

    cout<<"thread is done"<<endl;
    return true;
}

void MyFFmpeg::setFilename(string filename)
{
    this->filename = filename;
    cout <<"set:"<<this->filename <<endl;
}

bool MyFFmpeg::CloseUrl()
{
    avformat_close_input(&pFormatCtx);

    return true;
}

void MyFFmpeg::avDumpFormat()
{
    av_dump_format(pFormatCtx, 0, filename.c_str(), 0); //输出视频信息
}

void MyFFmpeg::run()
{
    //setFilename("E:\\testvideo\\Titanic.mkv");
    OpenUrl();
}

int MyFFmpeg::avReadFrame(CAVPacket cpacket)
{
    return av_read_frame(pFormatCtx,cpacket.getAVPacket());
}

void MyFFmpeg::play()
{
    if(running)
    {
        if(this->isplay)
            this->isplay = false;
        else
            this->isplay = true;

    }
    else
    {
        isplay = true;
        running = true;
        this->start();
    }
}

void MyFFmpeg::stop()
{
    running = false;
}

string MyFFmpeg::getFilename() const
{
    return filename;
}
