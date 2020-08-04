//
// Created by wu on 2020/8/3.
//

#include "WIAudio.h"

WIAudio::WIAudio(WIPlayStatus *playStatus) : playStatus(playStatus) {
    queue = new WIQueue(playStatus);
    buffer = (uint8_t *) (av_malloc(44100 * 2 * 2));//每秒的pcm数据
}


WIAudio::~WIAudio() {

}

//线程回调方法
void *decodePlay(void *data) {
    WIAudio *audio = (WIAudio *) (data);
    audio->resampleAudio();
    pthread_exit(&audio->playThread);
}


void WIAudio::play() {
    //创建线程
    pthread_create(&playThread, NULL, decodePlay, this);
}

FILE *outFile = fopen("/storage/emulated/0/Charlie Puth - Look At Me Now.pcm", "w");


int WIAudio::resampleAudio() {
    int ret;
    while (playStatus != NULL && !playStatus->exit) {
        avPacket = av_packet_alloc();
        if (queue->getAvPacket(avPacket) != 0) {
            av_packet_free(&avPacket);
            av_free(&avPacket);
            avPacket = NULL;
            continue;
        }
        ret = avcodec_send_packet(pACodecCtx, avPacket);
        if (ret != 0) {
            av_packet_free(&avPacket);
            av_free(&avPacket);
            avPacket = NULL;
            continue;
        }
        avFrame = av_frame_alloc();
        ret = avcodec_receive_frame(pACodecCtx, avFrame);
        if (ret == 0) {//进行重新采样
            if (avFrame->channels > 0 && avFrame->channel_layout == 0) {
                //有声道数没有声道布局，所以要设置声道布局
                avFrame->channel_layout = av_get_default_channel_layout(avFrame->channels);
            } else if (avFrame->channels == 0 && avFrame->channel_layout > 0) {
                //有声道布局没有声道数，所以要设置声道数
                avFrame->channels = av_get_channel_layout_nb_channels(avFrame->channel_layout);
            }
            SwrContext *swr_ctx = NULL;
            swr_ctx = swr_alloc_set_opts(NULL,
                                         AV_CH_LAYOUT_STEREO,// 输出声道布局:立体声
                                         AV_SAMPLE_FMT_S16,//输出采样位数格式
                                         avFrame->sample_rate,//输出的采样率
                                         avFrame->channel_layout,//输入声道布局
                                         (AVSampleFormat) (avFrame->format),//输入采样位数格式
                                         avFrame->sample_rate,//输入采样率
                                         NULL,
                                         NULL);
            if (!swr_ctx || swr_init(swr_ctx) < 0) {
                av_packet_free(&avPacket);
                av_freep(avPacket);
                avPacket = NULL;

                av_frame_free(&avFrame);
                av_free(avFrame);
                avFrame = NULL;

                if (swr_ctx != NULL) {
                    swr_free(&swr_ctx);
                    swr_ctx = NULL;
                }
                LOGE("!swr_ctx || swr_init(swr_ctx) < 0");
                continue;
            }

            //返回的是采样的个数
            int nb = swr_convert(swr_ctx,
                                 &buffer,//转码后输出的PCM数据大小
                                 avFrame->nb_samples,//输出采样个数
                                 (const uint8_t **) (avFrame->data),
                                 avFrame->nb_samples);

            int out_channels = av_get_channel_layout_nb_channels(AV_CH_LAYOUT_STEREO);
            data_size = nb * out_channels * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);
            fwrite(buffer, 1, data_size, outFile);
            LOGD("DATA SIZE == %d", data_size);
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;

            av_frame_free(&avFrame);
            av_free(avFrame);
            avFrame = NULL;

            swr_free(&swr_ctx);
            swr_ctx = NULL;
        } else {
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            av_frame_free(&avFrame);
            av_free(avFrame);
            avFrame = NULL;
            continue;
        }
    }
    return 0;
}
