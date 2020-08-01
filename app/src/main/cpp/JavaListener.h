#ifndef CDEMO_JAVALISTENER_H
#define CDEMO_JAVALISTENER_H

#include "jni.h"
class JavaListener {
public:
    JavaVM *javaVm;
    JNIEnv *jniEnv;
    jobject jobj;
    jmethodID  methodId;
public:
    JavaListener(JavaVM *vm,JNIEnv *env,jobject obj);
    ~JavaListener();

public:
    /**
     *
     * @param type 0：主线程  1：其他线程
     * @param code
     * @param msg
     */
    void onError(int type,int code, const char *msg);
};

#endif //CDEMO_JAVALISTENER_H
