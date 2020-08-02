//
// Created by wu on 2020/7/31.使用OpenSLES进行音频的播放
// 参考地址https://www.jianshu.com/p/063cce0a5ed0
//
#include <cstdio>
#include <malloc.h>
#include "jni.h"
#include "SLES/OpenSLES.h"
#include "SLES/OpenSLES_Android.h"
#include "android_log.h"

SLObjectItf engineObject = NULL;//引擎接口对象
SLEngineItf engineItf = NULL;//具体的引擎对象实例
SLObjectItf outputMixItf;
SLObjectItf bqPlayerObject = 0;//播放器
SLPlayItf bqPlayerPlay = 0;//播放器接口
SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue = 0;//播放器队列接口

FILE *pcmFile;
void *buffer;
uint8_t *out_buffer;

int getPcmData(void **pcm) {
    int size = 0;
    while (!feof(pcmFile)) {
        size = fread(out_buffer, 1, 44100 * 2 * 2, pcmFile);
        if (out_buffer == NULL) {
            LOGE("pcm file read end");
        } else {
            LOGE("pcm file is reading");
        }
        *pcm = out_buffer;
        break;
    }
    return size;
}

/**
 * 播放器回调函数
 * @param bq
 * @param context
 */
void bqPlayerCallBack(SLAndroidSimpleBufferQueueItf bq, void *context) {
   int size= getPcmData(&buffer);
    if (buffer != NULL) {
        //这里会将buffer的数据塞入到队列中，然后会自动进行播放，播放完成以后，会再次调用这个回调函数。只到数据播放完成
        (*bqPlayerBufferQueue)->Enqueue(bqPlayerBufferQueue,buffer,size);
    }
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_cdemo_MainActivity_playprm(JNIEnv *env, jobject thiz, jstring path) {
    const char *url = env->GetStringUTFChars(path, 0);
    LOGD("文件位置是：%s",url);
    pcmFile = fopen("/sdcard/audio_long8.pcm", "r");
    if (pcmFile == NULL) {
        LOGE("获取到的文件为空%s",url);
        return;
    }
    out_buffer = static_cast<uint8_t *>(malloc(41100 * 2 * 2));
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
    SLDataFormat_PCM formatPcm = {SL_DATAFORMAT_PCM, 2, SL_SAMPLINGRATE_44_1,
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
    (*bqPlayerBufferQueue)->RegisterCallback(bqPlayerBufferQueue, bqPlayerCallBack, NULL);
    /**
     * 设置播放器状态为播放
     */
    (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_PLAYING);
    /**
     * 手动激活回调函数
     */
    bqPlayerCallBack(bqPlayerBufferQueue, NULL);
}