//
// Created by Administrator on 2020/8/2.
//

#ifndef CDEMO_WIPLAYER_H
#define CDEMO_WIPLAYER_H
//这里一定要注意，因为ffmpeg是通过c++编译的，一定要使用这种方式来包括引入的ffmpeg的头文件，否则会导致找不到h对应的函数
extern "C"
{
#include "include/libavcodec/avcodec.h"
#include "include/libavformat/avformat.h"
#include "include/libavutil/avutil.h"
}
#include "WIPreparedListener.h"
#include "pthread.h"
#include "WIAudio.h"

/**
 * 进行对应的FFmpeg编码解码工作
 */
class WIPlayer {
public:
    WIPreparedListener *call = NULL;//初始化回调Java方法的封装类
    const char *url = NULL;//文件的url地址
    pthread_t decodeThread;//解码的子线程
    AVFormatContext *av_format_ctx = NULL;//封装的上下文
    WIAudio *audio = NULL;//封装的Audio信息
    WIPlayStatus *playStatus=NULL;
public:
    WIPlayer(WIPreparedListener *call, const char *url, WIPlayStatus *playStatus);

    virtual ~WIPlayer();

    void prepare();

    void decodeAudioThread();

    void start();
};


#endif //CDEMO_WIPLAYER_H
