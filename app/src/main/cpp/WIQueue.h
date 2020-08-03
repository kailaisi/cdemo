//
// Created by wu on 2020/8/3.
//
#include "android_log.h"
#include "WIPlayStatus.h"
#include "pthread.h"
#include <queue>

extern "C" {
#include "include/libavcodec/avcodec.h"
}
#ifndef CDEMO_WIQUEUE_H
#define CDEMO_WIQUEUE_H

using namespace std;

//由于解码获取AVPacket需要耗费一定的时间，为了能够达到更好的播放效果。
//这里需要将解码出来的AvPacket缓存到队列中，然后播放的时候直接从队列中去获取
class WIQueue {
public:
    //avpacket队列
    queue<AVPacket *> queuepacket;
    //线程锁
    pthread_mutex_t mutexPacket;
    //通知
    pthread_cond_t condPacket;
    //播放状态
    WIPlayStatus *playState = NULL;
public:
    WIQueue(WIPlayStatus *playState);

    virtual ~WIQueue();
    int putAvPacket(AVPacket *avPacket);
    int getAvPacket(AVPacket *avPacket);
    int getQueueSize();
};


#endif //CDEMO_WIQUEUE_H
