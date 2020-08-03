//
// Created by Administrator on 2020/8/2.
//

#include "jni.h"
#ifndef CDEMO_JFCALLJAVA_H
#define CDEMO_JFCALLJAVA_H
#define MAIN_THREAD 0
#define CHILD_THREAD 1
class WIPreparedListener {
public:
    JavaVM *javaVm=NULL;
    JNIEnv *jniEnv=NULL;
    jobject  jobj;
    jmethodID  jmethodID;
public:
    WIPreparedListener(JavaVM *javaVm, JNIEnv *jniEnv, jobject *jobj);

    virtual ~WIPreparedListener();
    /**
     * 这里会调用Java层的方法，可能在主线程，也可能在子线程
     * @param threadType
     */
    void onCallPrepared(int threadType);
};


#endif //CDEMO_JFCALLJAVA_H
