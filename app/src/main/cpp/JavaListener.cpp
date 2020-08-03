//
// Created by wu on 2020/7/31.
//

#include "JavaListener.h"
#include "android_log.h"
#include "global.h"
JavaListener::JavaListener(JavaVM *vm, JNIEnv *env, jobject obj) {
    javaVm = vm;
    jniEnv = env;
    jobj = obj;
    jclass clazz = env->GetObjectClass(jobj);
    if (clazz == NULL) {
        return;
    }
    methodId = jniEnv->GetMethodID(clazz, "onError", "(ILjava/lang/String;)V");
}

void JavaListener::onError(int type, int code, const char *msg) {
    if (type == 0) {//
        jstring jmsg = jniEnv->NewStringUTF(msg);
        jniEnv->CallVoidMethod(jobj, methodId, code, jmsg);
        jniEnv->DeleteLocalRef(jmsg);
    } else {
        if(globalJvm==NULL){
            LOGE("系统没有获取到对应的java vm");
            return;
        }
        JNIEnv *env;
        //获取当前线程
        globalJvm->AttachCurrentThread(&env, 0);
        jstring  jmsg=env->NewStringUTF(msg);
        env->CallVoidMethod(jobj, methodId, code, jmsg);
        env->DeleteLocalRef(jmsg);
        javaVm->DetachCurrentThread();
    }
}

JavaListener::~JavaListener() {
}


#include "pthread.h"

JavaListener *javaListener;
pthread_t childThread;

void *childThreadCall(void *data) {
    JavaListener *listener = (JavaListener *) (data);
    listener->onError(1, 100, "c++ call java method from child thread");
    pthread_exit(&childThread);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_cdemo_ThreadDemo_callBack(JNIEnv *env, jobject thiz) {
    if(globalJvm==NULL){
        LOGE("系统没有获取到对应的java vm");
    }
    javaListener = new JavaListener(globalJvm, env, env->NewGlobalRef(thiz));
    //主线程调用
     javaListener->onError(0, 100, "c++ call java method from main thread");
    //子线程调用
    pthread_create(&childThread, NULL, childThreadCall, javaListener);
}