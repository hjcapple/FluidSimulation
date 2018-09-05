#ifndef __GLUTILS_LOGUTILS_H___
#define __GLUTILS_LOGUTILS_H___

namespace glutils {
    typedef enum {
        LogLevel_Info = 0x00000001,
        LogLevel_Warn = 0x00000002,
        LogLevel_Error = 0x00000004,
        LogLevel_Debug = 0x00000008,
        LogLevel_Verbose = 0xFFFFFFFF
    } LogLevel;

    typedef void (*LogCallback)(const char *fmtMsg);

    void _LogVerbose(const char *sTag, const char *fmtMsg, ...);
    void _LogDebug(const char *sTag, const char *fmtMsg, ...);
    void _LogInfo(const char *sTag, const char *fmtMsg, ...);
    void _LogWarn(const char *sTag, const char *fmtMsg, ...);
    void _LogError(const char *sTag, const char *fmtMsg, ...);

} // namespace glutils

#define GLUTILS_LOGV(...) glutils::_LogVerbose("GLUtils", __VA_ARGS__)
#define GLUTILS_LOGD(...) glutils::_LogDebug("GLUtils", __VA_ARGS__)
#define GLUTILS_LOGI(...) glutils::_LogInfo("GLUtils", __VA_ARGS__)
#define GLUTILS_LOGW(...) glutils::_LogWarn("GLUtils", __VA_ARGS__)
#define GLUTILS_LOGE(...) glutils::_LogError("GLUtils", __VA_ARGS__)

#endif
