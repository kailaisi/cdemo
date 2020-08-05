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
    //通知消费者
    pthread_cond_signal(&condPacket);
    //解锁
    pthread_mutex_unlock(&mutexPacket);
    return 0;
}

int WIQueue::getAvPacket(AVPacket *packet) {
    pthread_mutex_lock(&mutexPacket);
    while (playState != NULL && !playState->exit){
        if (queuepacket.size() > 0){
            AVPacket *avPacket = queuepacket.front();//取出来
            if (av_packet_ref(packet,avPacket) == 0){//把pkt的内存数据拷贝到avPacket内存中，只是拷贝了引用
                queuepacket.pop();
            }
            av_packet_free(&avPacket);//AVPacket中的第一个参数，就是引用，减到0才真正释放
            av_free(avPacket);
            avPacket = NULL;
            if (LOG_DEBUG){
                LOGD("从队列中取出一个AVPacket，还剩下%d个",queuepacket.size());
            }
            break;
        } else {
            pthread_cond_wait(&condPacket,&mutexPacket);
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
