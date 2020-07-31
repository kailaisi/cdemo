//
// Created by wu on 2020/7/31.使用OpenSLES进行音频的播放
//
#include "jni.h"
#include "SLES/OpenSLES.h"
#include "SLES/OpenSLES_Android.h"

SLObjectItf engineObject=NULL;//引擎接口对象
SLEngineItf engineItf=NULL;//具体的引擎对象实例


extern "C"
JNIEXPORT void JNICALL
Java_com_example_cdemo_MainActivity_playprm(JNIEnv *env, jobject thiz, jstring path) {
    const char *url=env->GetStringUTFChars(path,0);
    //创建引擎
    SLresult  result;
    result=slCreateEngine(&engineObject,0,NULL,0,NULL,NULL);
    //实现Realize engineObject接口对象
    (*engineObject)->Realize(engineObject,SL_BOOLEAN_FALSE);
    //初始化engineEngine对象
    (*engineObject)->GetInterface(engineObject,SL_IID_ENGINE,&engineItf);

    env->ReleaseStringUTFChars(path,url)
}