#include "logutils.h"
#include <stdio.h>
#include <stdarg.h>
#include <cstddef>

#ifdef OF_ANDROID
#include <android/log.h>
#endif // OF_ANDROID

namespace glutils {

#define MAX_LOGBUFFER_LEN 2048
    static char s_aStringBuf[MAX_LOGBUFFER_LEN];
    LogCallback g_fnLog = nullptr;
    int32_t g_logLevel = LogLevel_Error | LogLevel_Warn | LogLevel_Info;

    void _LogVerbose(const char *sTag, const char *fmtMsg, ...) {
#ifdef OF_DEBUG
        if (g_logLevel == OF_LogLevel_Verbose) {
            va_list vaList;
            va_start(vaList, fmtMsg);
            vsnprintf(s_aStringBuf, MAX_LOGBUFFER_LEN, fmtMsg, vaList);
            va_end(vaList);

            if (g_fnLog) {
                g_fnLog(s_aStringBuf);
            } else {
#ifdef OF_ANDROID
                __android_log_print(ANDROID_LOG_VERBOSE, sTag, "%s", s_aStringBuf);
#else
                printf("[%s-%s]: ", sTag, "Verbose");
                printf("%s", s_aStringBuf);
                printf("\n");
#endif
            }
        }
#endif
    }

    void _LogDebug(const char *sTag, const char *fmtMsg, ...) {
        if (g_logLevel & LogLevel_Debug) {
            va_list vaList;
            va_start(vaList, fmtMsg);
            vsnprintf(s_aStringBuf, MAX_LOGBUFFER_LEN, fmtMsg, vaList);
            va_end(vaList);

            if (g_fnLog) {
                g_fnLog(s_aStringBuf);
            } else {
#ifdef OF_ANDROID
                __android_log_print(ANDROID_LOG_DEBUG, sTag, "%s", s_aStringBuf);
#else
                printf("[%s-%s]: ", sTag, "Debug");
                printf("%s", s_aStringBuf);
                printf("\n");
#endif
            }
        }
    }

    void _LogInfo(const char *sTag, const char *fmtMsg, ...) {
        if (g_logLevel & LogLevel_Info) {
            va_list vaList;
            va_start(vaList, fmtMsg);
            vsnprintf(s_aStringBuf, MAX_LOGBUFFER_LEN, fmtMsg, vaList);
            va_end(vaList);
            if (g_fnLog) {
                g_fnLog(s_aStringBuf);
            } else {
#ifdef OF_ANDROID
                __android_log_print(ANDROID_LOG_INFO, sTag, "%s", s_aStringBuf);
#else
                printf("[%s-%s]: ", sTag, "Info");
                printf("%s", s_aStringBuf);
                printf("\n");
#endif
            }
        }
    }

    void _LogWarn(const char *sTag, const char *fmtMsg, ...) {
        if (g_logLevel & LogLevel_Warn) {
            va_list vaList;
            va_start(vaList, fmtMsg);
            vsnprintf(s_aStringBuf, MAX_LOGBUFFER_LEN, fmtMsg, vaList);
            va_end(vaList);
            if (g_fnLog) {
                g_fnLog(s_aStringBuf);
            } else {
#ifdef OF_ANDROID
                __android_log_print(ANDROID_LOG_WARN, sTag, "%s", s_aStringBuf);
#else
                printf("[%s-%s]: ", sTag, "Warn");
                printf("%s", s_aStringBuf);
                printf("\n");
#endif
            }
        }
    }

    void _LogError(const char *sTag, const char *fmtMsg, ...) {
        if (g_logLevel & LogLevel_Error) {
            va_list vaList;
            va_start(vaList, fmtMsg);
            vsnprintf(s_aStringBuf, MAX_LOGBUFFER_LEN, fmtMsg, vaList);
            va_end(vaList);
            if (g_fnLog) {
                g_fnLog(s_aStringBuf);
            } else {
#ifdef OF_ANDROID
                __android_log_print(ANDROID_LOG_ERROR, sTag, "%s", s_aStringBuf);
#else
                printf("[%s-%s]: ", sTag, "Error");
                printf("%s", s_aStringBuf);
                printf("\n");
#endif
            }
        }
    }
} // namespace OrangeFilter
