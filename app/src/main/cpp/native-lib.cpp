#include <jni.h>
#include <string>
#include <android/log.h>
#define TAG "native Info"
#ifndef LOG_TAG
#define LOG_TAG "HELL_JNI"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,LOG_TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,LOG_TAG ,__VA_ARGS__) // 定义LOGF类型
#endif
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_cdemo_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {
    std::string hello = "Hello from C++";
    //获取thiz的class，也就是java中的Class信息
    jclass thisclazz = env->GetObjectClass(thiz);
    //获取getClass方法的methodId
    jmethodID jmethodId = env->GetMethodID(thisclazz, "getClass", "()Ljava/lang/Class;");
    //执行getClass方法，获取Class对象
    jobject instance = env->CallObjectMethod(thisclazz, jmethodId);
    //获取Class实例
    jclass clazz=env->GetObjectClass(instance);
    //获取其getName方法id
    jmethodID mid_getName=env->GetMethodID(clazz,"getName","()Ljava/lang/String;");
    //调用getName方法
    auto  name=static_cast<jstring >(env->CallObjectMethod(instance,mid_getName));
    LOGD("class name:%s",env->GetStringUTFChars(name,0));
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_cdemo_MainActivity_setArray(JNIEnv *env, jobject thiz, jintArray int_array_of) {
    jint  len=env->GetArrayLength(int_array_of);
    LOGE("array.len =%d",len);

    jboolean isCopy;
    //获取数组地址。
    //第二个参数标识javaArray->c/c++ Array转换的方式
    jint  *firstElement=env->GetIntArrayElements(int_array_of,&isCopy);
    LOGD("is copy array:%d",isCopy);
    //遍历数组
    for (int i = 0; i < len; ++i) {
        LOGD("array[%i]=%i",i,*(firstElement+i));
    }
    //使用后释放数组
    env->ReleaseIntArrayElements(int_array_of,firstElement,0);
    jintArray  newArray=env->NewIntArray(3);
    return env->NewStringUTF("ssss");
}extern "C"
JNIEXPORT void JNICALL
Java_com_example_cdemo_MainActivity_testFIeld(JNIEnv *env, jobject thiz) {
    jclass  jclass1=env->GetObjectClass(thiz);
    jfieldID  jfieldId=env->GetFieldID(jclass1,"testField", "Ljava/lang/String;");
    jstring jstr=static_cast<jstring>(env->GetObjectField(thiz,jfieldId));
    const char* cStr=env->GetStringUTFChars(jstr,NULL);
    LOGE("获取到的原本的数据是：%s",cStr);
    //修改原来的值
    jstring newValue=env->NewStringUTF("新的字符串");
    env->SetObjectField(thiz,jfieldId,newValue);
    //释放资源
    env->ReleaseStringUTFChars(jstr,cStr);
}