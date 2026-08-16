#ifndef LOG_H
#define LOG_H

#include <android/log.h>

#define TAG "NoDex-Native"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

#endif // LOG_H
