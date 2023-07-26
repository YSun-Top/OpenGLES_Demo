//
// Created by Silen on 2023/6/15.
// 日志打印
//

#ifndef LEARNINGPROJECT_LOGUTILS_H
#define LEARNINGPROJECT_LOGUTILS_H

#include <android/log.h>
//当使用场景是C++且是Android时
#if (__cplusplus) && (ANDROID)

#include <initializer_list>
#include <string>

using std::initializer_list;
using std::string;

template<typename T>
void LogD(const string &tag, const string &msg, initializer_list<T> args = {}) {
    __android_log_print(ANDROID_LOG_DEBUG, tag.c_str(), msg.c_str(), args);
}
template<typename T>
void LogD(const string &tag, const string &msg, T args) {
    __android_log_print(ANDROID_LOG_DEBUG, tag.c_str(), msg.c_str(), args);
}

template<typename T>
void LogW(const string &tag, const string &msg, initializer_list<T> args = {}) {
    __android_log_print(ANDROID_LOG_WARN, tag.c_str(), msg.c_str(), args);
}
template<typename T>
void LogW(const string &tag, const string &msg, T args) {
    __android_log_print(ANDROID_LOG_WARN, tag.c_str(), msg.c_str(), args);
}

template<typename T>
void LogI(const string &tag, const string &msg, initializer_list<T> args = {}) {
    __android_log_print(ANDROID_LOG_INFO, tag.c_str(), msg.c_str(), args);
}
template<typename T>
void LogI(const string &tag, const string &msg, T args) {
    __android_log_print(ANDROID_LOG_INFO, tag.c_str(), msg.c_str(), args);
}

template<typename T>
void LogE(const string &tag, const string &msg, initializer_list<T> args = {}) {
    __android_log_print(ANDROID_LOG_ERROR, tag.c_str(), msg.c_str(), args);
}
template<typename T>
void LogE(const string &tag, const string &msg, T args) {
    __android_log_print(ANDROID_LOG_ERROR, tag.c_str(), msg.c_str(), args);
}

#else
#define LOGD(tag,format, ...)  __android_log_print(ANDROID_LOG_DEBUG,  tag, format, ##__VA_ARGS__)
#define LOGW(tag,format, ...)  __android_log_print(ANDROID_LOG_WARN,  tag, format, ##__VA_ARGS__)
#define LOGI(tag,format, ...)  __android_log_print(ANDROID_LOG_INFO,  tag, format, ##__VA_ARGS__)
#define LOGE(tag,format, ...)  __android_log_print(ANDROID_LOG_ERROR, tag, format, ##__VA_ARGS__)
#endif

#endif //LEARNINGPROJECT_LOGUTILS_H
