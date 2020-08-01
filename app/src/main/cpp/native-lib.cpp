#include <jni.h>
#include <string>
#include <android/log.h>
#include "extra.h"
#include "android_log.h"
#include "JavaListener.h"

extern "C" JNIEXPORT jstring
Java_com_example_cdemo_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {
    std::string hello = getString();
    //获取thiz的class，也就是java中的Class信息
    jclass thisclazz = env->GetObjectClass(thiz);
    //获取getClass方法的methodId
    jmethodID jmethodId = env->GetMethodID(thisclazz, "getClass", "()Ljava/lang/Class;");
    //执行getClass方法，获取Class对象
    jobject instance = env->CallObjectMethod(thisclazz, jmethodId);
    //获取Class实例
    jclass clazz = env->GetObjectClass(instance);
    //获取其getName方法id
    jmethodID mid_getName = env->GetMethodID(clazz, "getName", "()Ljava/lang/String;");
    //调用getName方法
    jstring name = static_cast<jstring >(env->CallObjectMethod(instance, mid_getName));
    LOGD("class name:%s", env->GetStringUTFChars(name, 0));
    //资源的释放
    env->DeleteLocalRef(thisclazz);
    env->DeleteLocalRef(clazz);
    env->DeleteLocalRef(instance);
    env->DeleteLocalRef(name);
    return env->NewStringUTF(hello.c_str());
}
extern "C" JNIEXPORT jstring
Java_com_example_cdemo_MainActivity_setArray(JNIEnv *env, jobject thiz, jintArray int_array_of) {
    jint len = env->GetArrayLength(int_array_of);
    LOGE("array.len =%d", len);

    jboolean isCopy;
    //获取数组地址。
    //第二个参数标识javaArray->c/c++ Array转换的方式
    jint *firstElement = env->GetIntArrayElements(int_array_of, &isCopy);
    LOGD("is copy array:%d", isCopy);
    //遍历数组
    for (int i = 0; i < len; ++i) {
        LOGD("array[%i]=%i", i, *(firstElement + i));
    }
    //使用后释放数组
    env->ReleaseIntArrayElements(int_array_of, firstElement, 0);
    jintArray newArray = env->NewIntArray(3);
    return env->NewStringUTF("ssss");
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_cdemo_MainActivity_testField(JNIEnv *env, jobject thiz) {
    jclass jclass1 = env->GetObjectClass(thiz);
    jfieldID jfieldId = env->GetFieldID(jclass1, "testField", "Ljava/lang/String;");
    jstring jstr = static_cast<jstring>(env->GetObjectField(thiz, jfieldId));
    const char *cStr = env->GetStringUTFChars(jstr, NULL);
    LOGE("获取到的原本的数据是：%s", cStr);
    //修改原来的值
    jstring newValue = env->NewStringUTF("新的字符串");
    env->SetObjectField(thiz, jfieldId, newValue);
    //释放资源
    env->ReleaseStringUTFChars(jstr, cStr);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_cdemo_MainActivity_printPerson(JNIEnv *env, jobject thiz) {
    jclass pClass = env->FindClass("com/example/cdemo/Person");
    //构造方法
    jmethodID constructID = env->GetMethodID(pClass, "<init>", "(ILjava/lang/String;)V");
    if (constructID == NULL) {
        return;
    }
    //创建名称
    jstring name = env->NewStringUTF("alex");
    //创建对象
    jobject person = env->NewObject(pClass, constructID, 1, name);
    jmethodID print = env->GetMethodID(pClass, "print", "()V");
    if (print == NULL) {
        return;
    }
    //调用对象的方法
    env->CallVoidMethod(person, print);
    //释放资源
    env->DeleteLocalRef(name);
    env->DeleteLocalRef(pClass);
    env->DeleteLocalRef(person);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_cdemo_JniUtils_remuxe(JNIEnv *env, jobject thiz, jstring inpath,
                                       jstring out_path) {
}
