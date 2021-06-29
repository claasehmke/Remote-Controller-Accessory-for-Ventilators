#include <jni.h>
#include <string>

#include "Store.h"

#include <android/log.h>

using namespace std;

extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    __android_log_print(ANDROID_LOG_INFO,  __FUNCTION__, "onLoad");
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }


    return JNI_VERSION_1_6;
}


//----------------- Setters -----------------
extern "C"
JNIEXPORT void JNICALL
Java_com_remote_controller_Store_setTime(
        JNIEnv* pEnv,
        jobject pThis,
        jint pInt32) {
    msg.time = pInt32;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_remote_controller_Store_setPushUpdate(
        JNIEnv* pEnv,
        jobject pThis,
        jint pInt32) {
    msg.push_update = pInt32;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_remote_controller_Store_setPush(
        JNIEnv* pEnv,
        jobject pThis,
        jint pInt32) {
    msg.push = pInt32;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_remote_controller_Store_setRotationUpdate(
        JNIEnv* pEnv,
        jobject pThis,
        jint pInt32) {
    msg.rotation_update = pInt32;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_remote_controller_Store_setRotation(
        JNIEnv* pEnv,
        jobject pThis,
        jint pInt32) {
    msg.rotation = pInt32;
}
//----------------- Getters -----------------
extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_remote_controller_Store_getByteArray(
        JNIEnv* pEnv,
        jobject pThis) {

    jbyteArray array;
    char* struct_as_array = reinterpret_cast<char*>(&msg);
    array = pEnv->NewByteArray(sizeof(MSG_ACTUATOR_CONTROL));
    //__android_log_write(ANDROID_LOG_INFO, "struct_as_array", std::to_string(sizeof(MSG_ACTUATOR_CONTROL)).c_str());

    pEnv->SetByteArrayRegion(array, 0, sizeof(MSG_ACTUATOR_CONTROL), (jbyte*) struct_as_array);

    return array;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_remote_controller_Store_getTime(
        JNIEnv* pEnv,
        jobject pThis) {
    return msg.time;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_remote_controller_Store_getRotation(
        JNIEnv* pEnv,
        jobject pThis) {
    return msg.rotation;
}