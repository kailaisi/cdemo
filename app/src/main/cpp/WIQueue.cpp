//
// Created by wu on 2020/8/3.
//

#include "WIQueue.h"

WIQueue::WIQueue(WIPlayStatus *playState) : playState(playState) {
    //初始化线程锁和消息队列
    pthread_mutex_init(&mutexPacket, NULL);
    pthread_cond_init(&condPacket, NULL);
}

WIQueue::~WIQueue() {
    //线程锁和消息队列
    pthread_mutex_destroy(&mutexPacket);
    pthread_cond_destroy(&condPacket);
}

int WIQueue::putAvPacket(AVPacket *avPacket) {
    //加锁
    pthread_mutex_lock(&mutexPacket);
    //入队
    queuepacket.push(avPacket);
    LOGE("packet队列增加一个数据，现在的个数为:%d", queuepacket.size());
    //通知消费者
    pthread_cond_signal(&condPacket);
    //解锁
    pthread_mutex_unlock(&mutexPacket);
    return 0;
}

int WIQueue::getAvPacket(AVPacket *avPacket) {
    pthread_mutex_lock(&mutexPacket);
    while (playState != NULL && !playState->exit) {
        if (queuepacket.size() > 0) {
            AVPacket *pkt = queuepacket.front();
            //将pkt中的数据赋值给avPacket，这里不是复制数据，而是复制的指针。
            if (av_packet_ref(avPacket, pkt) == 0) {
                queuepacket.pop();
            }
            //将队列中的指针内存释放。但是这里指针事项的数据并不会被释放掉
            av_packet_free(&pkt);
            av_freep(pkt);
            pkt = NULL;
            LOGE("从packet队列中取出一个packet，队列中剩余数量为%d", queuepacket.size());
            break;
        } else {
            //没有数据，iinxing dengdai
            pthread_cond_wait(&condPacket, &mutexPacket);
        }
    }
    pthread_mutex_unlock(&mutexPacket);
    return 0;
}

int WIQueue::getQueueSize() {
    int size = 0;
    pthread_mutex_lock(&mutexPacket);
    size = queuepacket.size();
    pthread_mutex_unlock(&mutexPacket);
    return size;
}
