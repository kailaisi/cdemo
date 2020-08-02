//
// Created by Administrator on 2020/8/2.
//

#include "JfCallJava.h"
#include "android_log.h"

JfCallJava::JfCallJava(JavaVM *javaVm, JNIEnv *jniEnv, jobject *obj) {
    this->javaVm = javaVm;
    this->jniEnv = jniEnv;
    this->jobj = jniEnv->NewGlobalRef(*obj);
    jclass jclz = jniEnv->GetObjectClass(jobj);
    if (!jclz) {
        LOGE("get jclass error");
        return;
    }
    jmethodID = jniEnv->GetMethodID(jclz, "onCallPrepared", "()V");

}

JfCallJava::~JfCallJava() {

}

void JfCallJava::onCallPrepared(int threadType) {
    if (threadType == MAIN_THREAD) {
        jniEnv->CallObjectMethod(jobj, jmethodID);
    } else {
        JNIEnv *env;
        int result = javaVm->AttachCurrentThread(&env, 0);
        if (result != JNI_OK) {
            LOGE("GET CHILD THREAD JNIENV ERROR");
            return;
        }
        env->CallObjectMethod(jobj, jmethodID);
        javaVm->DetachCurrentThread();
    }
}

