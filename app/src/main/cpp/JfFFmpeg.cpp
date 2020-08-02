//
// Created by Administrator on 2020/8/2.
//

#include "JfFFmpeg.h"

JfFFmpeg::JfFFmpeg(JfCallJava *call, const char *url) : call(call), url(url) {

}

JfFFmpeg::~JfFFmpeg() {

}

/**
 * 线程创建以后的回调函数
 * @param data
 * @return
 */
void *decodeFFmpeg(void* data){
    JfFFmpeg *jfFFmpeg=(JfFFmpeg*)(data);
    jfFFmpeg->decodeAudioThread();
    pthread_exit(&jfFFmpeg->decodeThread)
}

void JfFFmpeg::prepare() {
    pthread_create(&decodeThread,NULL,decodeFFmpeg,this)
}

void JfFFmpeg::decodeAudioThread() {

}
