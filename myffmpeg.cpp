#include "myffmpeg.h"


  list<AVFrame*> audioList ;
 struct SwrContext  *au_convert_ctx;

 uint8_t * out_buffer;
  uint8_t * out_buffer_a;

  int out_buffer_size_A;


#define MAX_AUDIO_FRAME_SIZE 192000
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
    //cout<<__FUNCTION__<<__LINE__<<endl;
    if (pCodec == NULL) {
        printf("Codec not found.\n");
        return false;
    }
   cout << pCodec->name << endl;
   ///打开解码器
   if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
       printf("Could not open codec.\n");
       return false;
   }

   pCodecCtx_A = pFormatCtx->streams[audioStream]->codec;
   pCodec_A = avcodec_find_decoder(pCodecCtx_A->codec_id);
   if (pCodec_A == NULL) {
       printf("pCodec_A not found.\n");
       return false;
   }
   cout << pCodec_A->name << endl;
   if(avcodec_open2(pCodecCtx_A,pCodec_A,NULL) < 0)
   {
       cout << "count not open codec." << endl;
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


    au_convert_ctx = swr_alloc();
    au_convert_ctx=swr_alloc_set_opts(au_convert_ctx,AV_CH_LAYOUT_STEREO, AV_SAMPLE_FMT_S16, 44100,
    av_get_default_channel_layout(pCodecCtx_A->channels),pCodecCtx->sample_fmt , pCodecCtx->sample_rate,0, NULL);
    swr_init(au_convert_ctx);

    out_buffer_a=(uint8_t *)av_malloc(MAX_AUDIO_FRAME_SIZE*2);
     out_buffer_size_A = av_samples_get_buffer_size(NULL,av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO) ,pCodecCtx_A->frame_size,AV_SAMPLE_FMT_S16, 1);
    int got_picture;

    initSDL();
    cout<<__FUNCTION__<<__LINE__<<endl;
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
        else if(packet->stream_index == audioStream)
        {
            int ret = avcodec_decode_audio4(pCodecCtx_A,pFrame, &got_picture, packet);
            if(ret < 0)
            {
                cout <<"decode error"<< endl;
            }
            if(got_picture)
            {

//                cout << "pFrame ->nb_samples = " << pFrame ->nb_samples << endl;
//


                AVFrame * avFrameTmp = av_frame_alloc();
                av_frame_copy(avFrameTmp, pFrame);
                audioList.push_back(avFrameTmp);
                //cout<< "get audio data" << endl;
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

void fill_audiodata(void *udata, uint8_t *stream, int len)
{
    static int bufflen = 0;

    SDL_memset(stream, 0 , len);




    if(bufflen <= 0)  //没有剩余数据了，就要开始出队列，转化格式了。
    {
        if(audioList.size() <= 0)
            return ;

        cout << "get data to audio len = "<< len << endl;
        AVFrame * avframe;
        audioList.push_front(avframe);

        swr_convert(au_convert_ctx,&out_buffer_a, MAX_AUDIO_FRAME_SIZE,(const uint8_t **)avframe->data ,\
                                   avframe->nb_samples);
        bufflen = out_buffer_size_A;


        //SDL_memcpy(stream,avframe->extended_data[0],len);
        av_frame_free(&avframe);
    }
    len = (len > bufflen ? bufflen : len);
    SDL_memcpy(stream,out_buffer+(out_buffer_size_A-bufflen),len);
    bufflen -= len;


}
int MyFFmpeg::initSDL()
{
    if(SDL_Init(SDL_INIT_AUDIO|SDL_INIT_TIMER))
    {
        cout << "sdl init error!" << endl;
    }



   // int out_sample_rate = 44100;
    wanted_spec.freq = 44100;
    wanted_spec.format = AUDIO_S16SYS;
    wanted_spec.channels = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
    wanted_spec.silence = 0;
    wanted_spec.samples = pCodecCtx_A->frame_size;
    wanted_spec.callback = fill_audiodata;
    wanted_spec.userdata = pCodecCtx_A;

    if(SDL_OpenAudio(&wanted_spec,NULL) < 0)
    {
        cout << "can't open audio" << endl;
        return -1;
    }
    SDL_PauseAudio(0);
    return 0;
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
