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
    audio->initSlES();
    pthread_exit(&audio->playThread);
}


void WIAudio::play() {
    //创建线程
    pthread_create(&playThread, NULL, decodePlay, this);
}

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
            if(LOG_DEBUG){
                LOGE("data size is %d",data_size);
            }
            LOGD("DATA SIZE == %d", data_size);
            av_packet_free(&avPacket);
            av_free(avPacket);
            avPacket = NULL;
            av_frame_free(&avFrame);
            av_free(avFrame);
            avFrame = NULL;
            swr_free(&swr_ctx);
            swr_ctx = NULL;
            break;
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
    return data_size;
}


/**
 * 播放器回调函数
 * @param bq
 * @param context
 */
void playCallBack(SLAndroidSimpleBufferQueueItf bq, void *context) {
    WIAudio *audio = (WIAudio *) context;
    if (audio != NULL) {
        int buffSize = audio->resampleAudio();
        if (buffSize > 0) {
            (*audio->bqPlayerBufferQueue)->Enqueue(audio->bqPlayerBufferQueue,audio->buffer,buffSize);
        }
    }
}


void WIAudio::initSlES() {
    /**
     * 创建引擎并获取引擎接口
     */
    SLresult result;
    //   1.1  创建引擎对象  engineObject
    result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
    if (result != SL_RESULT_SUCCESS) {
        return;
    }
    //    1.2  实现Realize engineObject接口对象  初始化引擎
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    if (result != SL_RESULT_SUCCESS) {
        return;
    }
    //    1.3  获取引擎接口   engineEngine对象
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineItf);
    if (result != SL_RESULT_SUCCESS) {
        return;
    }
    /**
     * 设置混音器
     */
    //2.1 创建混音器
    result = (*engineItf)->CreateOutputMix(engineItf, &outputMixItf, 0, 0, 0);
    if (result != SL_RESULT_SUCCESS) {
        return;
    }
    //初始化
    result = (*outputMixItf)->Realize(outputMixItf, SL_BOOLEAN_FALSE);
    if (result != SL_RESULT_SUCCESS) {
        return;
    }
    result = (*outputMixItf)->GetInterface(outputMixItf, SL_IID_ENVIRONMENTALREVERB,
                                           &outputMixEnvironmentalReverb);
    if (SL_RESULT_SUCCESS == result) {//设置环境的属性
        result = (*outputMixEnvironmentalReverb)->SetEnvironmentalReverbProperties(
                outputMixEnvironmentalReverb, &reverbSettings);
        (void) result;
    }
    /**
    * create player
    */
    //3.1  配置输入声音信息
    //创建buffer缓冲类型的队列  2个队列
    SLDataLocator_AndroidSimpleBufferQueue loc_bufq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
    //pcm数据格式
    //SL_DATAFORMAT_PCM：数据格式为pcm格式
    //2：双声道
    //SL_SAMPLINGRATE_44_1：采样率为44100
    //SL_PCMSAMPLEFORMAT_FIXED_16：采样格式为16bit
    //SL_PCMSAMPLEFORMAT_FIXED_16：数据大小为16bit
    //SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT：左右声道（双声道）
    //SL_BYTEORDER_LITTLEENDIAN：小端模式
    SLDataFormat_PCM formatPcm = {SL_DATAFORMAT_PCM,
                                  2,
                                  SL_SAMPLINGRATE_44_1,
                                  SL_PCMSAMPLEFORMAT_FIXED_16,
                                  SL_PCMSAMPLEFORMAT_FIXED_16,
                                  SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
                                  SL_BYTEORDER_LITTLEENDIAN};
    //数据源   将设置信息设置到数据源中
    SLDataSource audioSrc = {&loc_bufq, &formatPcm};
    //3.2 设置音轨（输出）
    //设置混音器
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixItf};
    SLDataSink audioSnk = {&loc_outmix, NULL};
    //需要的接口  操作队列的接口
    const SLInterfaceID ids[1] = {SL_IID_BUFFERQUEUE};
    const SLboolean req[1] = {SL_BOOLEAN_TRUE};
    //3.3 创建播放器
    result = (*engineItf)->CreateAudioPlayer(engineItf, &bqPlayerObject, &audioSrc, &audioSnk, 1,
                                             ids, req);
    if (result != SL_RESULT_SUCCESS) {
        return;
    }
    //3.4播放器初始化
    result = (*bqPlayerObject)->Realize(bqPlayerObject, SL_BOOLEAN_FALSE);
    if (result != SL_RESULT_SUCCESS) {
        return;
    }
    // 3.5 获取播放器接口
    result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_PLAY, &bqPlayerPlay);
    if (result != SL_RESULT_SUCCESS) {
        return;
    }
    /**
     * 设置播放器回调函数
     */
    //获取播放器队列接口
    (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_BUFFERQUEUE, &bqPlayerBufferQueue);
    //设置回调函数
    (*bqPlayerBufferQueue)->RegisterCallback(bqPlayerBufferQueue, playCallBack, NULL);
    //设置播放器状态为播放
    (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_PLAYING);
    //手动激活回调函数
    playCallBack(bqPlayerBufferQueue, this);
}
