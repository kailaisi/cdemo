//
// Created by wu on 2020/8/3.
//

#ifndef CDEMO_WIAUDIO_H
#define CDEMO_WIAUDIO_H


#include "include/libavcodec/avcodec.h"

class WIAudio {
public:
    int audio_stream_index;//音频流的索引号

    AVCodecParameters *av_codec_par=NULL;//音频流信息

    AVCodecContext *av_codec_ctx=NULL;//音频解码上下文
public:
    WIAudio();

    virtual ~WIAudio();
};


#endif //CDEMO_WIAUDIO_H
