#include <jni.h>
#include <string>
#include <android/log.h>
#include "android_log.h"
#define TAG "native Info"

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
    globalJvm=vm;
    /*获取JNIEnv,这里的第一个参数是二级指针*/
    int result = vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6);
    if (result != JNI_OK) {
        LOGE("获取Env失败");
        return JNI_VERSION_1_6;
    }
    /*注册方法的类*/
    jclass classMain = env->FindClass("com/example/cdemo/MainActivity");
    /*调用动态注册方法，将方法进行注册*/
    result = env->RegisterNatives(classMain, methods, 2);
    if (result != JNI_OK) {
        LOGE("动态方法注册失败");
        return JNI_VERSION_1_2;
    }
    return JNI_VERSION_1_6;
}