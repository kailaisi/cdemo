
#include "jni.h"
#include <string>
#include "pthread.h"
#include "android_log.h"

pthread_t thread;

void *normalCallBack(void *data){
    LOGE("create normal thread for C++");
    pthread_exit(&thread);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_cdemo_ThreadDemo_normalThread(JNIEnv *env, jobject thiz) {
    pthread_create(&thread,NULL,normalCallBack,NULL);
}

#include "queue"
#include "unistd.h"
#include "JavaListener.h"
pthread_t  produce;
pthread_t  custom;
pthread_mutex_t mutex;
pthread_cond_t conn;
std::queue<int> queue;

void *produceCallBack(void *){
    while(1){
        pthread_mutex_lock(&mutex);
        queue.push(1);
        LOGD("生产者生成一个数据，通知消费者进行消费。%d",queue.size());
        pthread_cond_signal(&conn);
        pthread_mutex_unlock(&mutex);
        sleep(10);
    }
    pthread_exit(&produce);
}

void *customCallBack(void *){
    while (1){
        pthread_mutex_lock(&mutex);
        if(!queue.empty()) {
            queue.pop();
            LOGD("消费者消费一个数据，数据还剩余%d 个。",queue.size());
        }else{
            LOGD("消费者正在等待消息");
            pthread_cond_wait(&conn,&mutex);
        }
        pthread_mutex_unlock(&mutex);
        usleep(500*1000);
    }
    pthread_exit(&custom);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_cdemo_ThreadDemo_mutexThread(JNIEnv *env, jobject thiz) {
    pthread_create(&produce,NULL,produceCallBack,NULL);
    pthread_create(&custom,NULL,customCallBack,NULL);
}
