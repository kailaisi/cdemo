//
// Created by Administrator on 2020/8/2.
//

#ifndef CDEMO_JFFFMPEG_H
#define CDEMO_JFFFMPEG_H

#include "JfCallJava.h"
#include "pthread.h"
#include "include/libavformat/avformat.h"

/**
 * 进行对应的FFmpeg编码解码工作
 */
class JfFFmpeg {
public:
    JfCallJava *call = NULL;//初始化回调Java方法的封装类
    const char *url = NULL;//文件的url地址
    pthread_t decodeThread = NULL;
    AVFormatContext *pAFmtCtx = NULL;//封装的上下文
    JfAudio *audio = NULL;//封装的Audio信息
public:
    JfFFmpeg(JfCallJava *call, const char *url);

    virtual ~JfFFmpeg();

    void prepare();

    void decodeAudioThread();
};


#endif //CDEMO_JFFFMPEG_H
