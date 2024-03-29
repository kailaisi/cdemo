//
// Created by Administrator on 2020/8/2.
//

#include "WIPlayer.h"
#include "android_log.h"

WIPlayer::WIPlayer(WIPreparedListener *call, const char *url, WIPlayStatus *playStatus) : call(
        call), url(url), playStatus(playStatus) {

}


WIPlayer::~WIPlayer() {

}

/**
 * 线程创建以后的回调函数
 * @param data
 * @return
 */
void *decodeFFmpeg(void *data) {
    WIPlayer *jfFFmpeg = (WIPlayer *) (data);
    jfFFmpeg->decodeAudioThread();
    return 0;
}


void WIPlayer::decodeAudioThread() {
    //注册所有组件并初始化网络
    av_register_all();
    avformat_network_init();
    av_format_ctx = avformat_alloc_context();
    int result = 0;
    //打开文件或者网络流
    result = avformat_open_input(&av_format_ctx, url, NULL, NULL);
    if (result != 0) {
        LOGE("can not open url:%s", url);
        return;
    }else{
        LOGE("open url: %s",url);
    }
    if (avformat_find_stream_info(av_format_ctx, NULL) < 0) {
        LOGE("can not find streams from %s", url);
        return;
    }
    //通过遍历，获取音频流的索引值和对应的音频流信息。
    for (int i = 0; i < av_format_ctx->nb_streams; i++) {
        AVCodecParameters *type = av_format_ctx->streams[i]->codecpar;
        if (type->codec_type == AVMEDIA_TYPE_AUDIO) {
            if (audio == NULL) {
                audio = new WIAudio(playStatus,type->sample_rate);
                audio->audio_stream_index = i;
                audio->av_codec_par = type;
            }
        }
    }
    //获取解码器
    AVCodec *decoder = avcodec_find_decoder(audio->av_codec_par->codec_id);
    if (decoder == NULL) {
        LOGE("can not find decoder from %s", url);
        return;
    }
    //申请解码上下文
    audio->pACodecCtx = avcodec_alloc_context3(decoder);
    if (!audio->pACodecCtx) {
        LOGE("can not alloc pACodecCtx");
        return;
    }
    //音频流信息填充到解码器
    if (avcodec_parameters_to_context(audio->pACodecCtx, audio->av_codec_par) < 0) {
        LOGE("can not parameters to context");
        return;
    }
    //打开解码器
    if (avcodec_open2(audio->pACodecCtx, decoder, 0) < 0) {
        LOGE("can not decoder from  %s", url);
        return;
    }
    if (call != NULL) {
        call->onCallPrepared(CHILD_THREAD);
    }

}

void WIPlayer::prepare() {
    pthread_create(&decodeThread, NULL, decodeFFmpeg, this);
}


void WIPlayer::start() {
    if (audio == NULL) {
        LOGE("audio is empty");
        return;
    }
    audio->play();
    int count = 0;
    while (playStatus != NULL && !playStatus->exit) {
        AVPacket *avPacket = av_packet_alloc();
        if (av_read_frame(av_format_ctx, avPacket) == 0) {
            if (avPacket->stream_index == audio->audio_stream_index) {
                count++;
                LOGE("正在解码第%d帧数据", count);
                //将解码后的帧数据放入到队列
                audio->queue->putAvPacket(avPacket);
            } else {
                av_packet_free(&avPacket);
                av_free(avPacket);
                avPacket = NULL;
            }
        } else {
            LOGE("结束解码第%d帧数据", count);
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            //队列中的avPacket还没有解码完
            while (playStatus != NULL && !playStatus->exit) {
                if (audio->queue->getQueueSize() > 0) {//把缓存中的avPacket也要释放出来
                    continue;
                } else {
                    playStatus->exit = true;
                    break;
                }
            }
        }
    }
    LOGE("解码完成");
}
