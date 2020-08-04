#include <jni.h>
#include <string>
#include <android/log.h>
#include "android_log.h"
#include "WIPlayer.h"

#define TAG "native Info"


JavaVM *javaVM = NULL;

WIPreparedListener *callJava = NULL;

WIPlayer *ffmpeg = NULL;

WIPlayStatus *playStatus = NULL;

void dynamicJavaFunc1() {
    LOGE("调用了 dynamicJavaFunc1");
}

jint dynamicJavaFunc2(JNIEnv *env, jobject thiz, jint i) {
    LOGE("调用了 dynamicJavaFunc2");
    return 666;
}

/*这里是需要动态注册的方法对应的数组*/
static const JNINativeMethod methods[] = {
        {"dynamicJavaFunc1", "()V",  (void *) dynamicJavaFunc1},
        {"dynamicJavaFunc2", "(I)I", (void *) dynamicJavaFunc2}
};
/*需要动态注册native方法的类名*/
static const char *mClassName = "com/example/cdemo/MainActivity";

#include "global.h"

jint JNI_OnLoad(JavaVM *vm, void *unused) {
    JNIEnv *env = NULL;
    javaVM = vm;
    globalJvm=vm;
    if (globalJvm == NULL) {
        LOGE("未获取到全局的JavaVM");
    } else {
        LOGE("获取到全局的JavaVM%p", vm);
    }
    /*获取JNIEnv,这里的第一个参数是二级指针*/
    int result = vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6);
    if (result != JNI_OK) {
        LOGE("获取Env失败");
        return JNI_VERSION_1_6;
    }
    /*注册方法的类*/
    jclass classMain = env->FindClass(mClassName);
    /*调用动态注册方法，将方法进行注册*/
    result = env->RegisterNatives(classMain, methods, 2);
    if (result != JNI_OK) {
        LOGE("动态方法注册失败");
        return JNI_VERSION_1_2;
    }
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_cdemo_jf_JfPlayer_n_1prepared(JNIEnv *env, jobject thiz, jstring source) {
    const char *url = env->GetStringUTFChars(source, 0);
    if (ffmpeg == NULL) {
        if (callJava == NULL) {
            callJava = new WIPreparedListener(javaVM, env, &thiz);
        }
        playStatus = new WIPlayStatus();
        ffmpeg = new WIPlayer(callJava, url, playStatus);
        ffmpeg->prepare();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_cdemo_jf_JfPlayer_n_1start(JNIEnv *env, jobject thiz) {
    ffmpeg->start();
}