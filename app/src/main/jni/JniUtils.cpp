//
// Created by xuzhiming02 on 2016/2/18.
//

#include "com_smile_sample_ndkdemo_JniUtils.h"

JNIEXPORT jstring JNICALL Java_com_smile_sample_ndkdemo_JniUtils_getStringFromC
        (JNIEnv * env, jclass obj){
    return env->NewStringUTF("这里是C++代码");
}
