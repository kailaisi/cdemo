//
// Created by wu on 2020/8/3.
//

#ifndef CDEMO_WIAUDIO_H
#define CDEMO_WIAUDIO_H
extern "C" {
#include "include/libavcodec/avcodec.h"
#include "include/libswresample/swresample.h"
}

#include "SLES/OpenSLES.h"
#include "SLES/OpenSLES_Android.h"
#include "pthread.h"
#include "WIPlayStatus.h"
#include "WIQueue.h"

/*https://www.jianshu.com/p/e28f606a3e03*/
class WIAudio {
public:
    int audio_stream_index;//音频流的索引号

    AVCodecParameters *av_codec_par = NULL;//音频流信息
    pthread_t playThread;
    WIPlayStatus *playStatus = NULL;
    AVPacket *avPacket;
    WIQueue *queue;
    AVCodecContext *pACodecCtx;//音频解码上下文
    AVFrame *avFrame;
    uint8_t *buffer;
    int data_size;


    SLObjectItf engineObject = NULL;//引擎接口对象
    SLEngineItf engineItf = NULL;//具体的引擎对象实例
    SLObjectItf outputMixItf;//混音器
    SLEnvironmentalReverbItf outputMixEnvironmentalReverb=NULL;//环境混音
    SLEnvironmentalReverbSettings reverbSettings=SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;
    SLObjectItf bqPlayerObject = 0;//播放器
    SLPlayItf bqPlayerPlay = 0;//播放器接口
    SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue = 0;//播放器队列接口
public:
    WIAudio(WIPlayStatus *playStatus);

    virtual ~WIAudio();

    //音频重采样方法
    int resampleAudio();

    void play();

    /**
     * 初始化播放器
     */
    void initSlES();

};


#endif //CDEMO_WIAUDIO_H
