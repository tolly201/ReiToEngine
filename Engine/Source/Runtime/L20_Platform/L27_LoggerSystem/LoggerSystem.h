#ifndef RT_LOGGERSYSTEM_H
#define RT_LOGGERSYSTEM_H
#include "../L23_SingletonFactory/RuntimeSingleton.h"
#include "../L26_File/Include.h"
#include "../L21_Logger/Logger.h"
#include "../L24_Objects/Include.h"
#include "../L24_Containers/Include.h"
namespace ReiToEngine
{
class RTLoggerSystem : public Runtime_Singleton<RTLoggerSystem>
{
    friend class Runtime_Singleton<RTLoggerSystem>;
public:
    RTLoggerSystem() = default;
    ~RTLoggerSystem() = default;
    b8 Initialize() override;
    b8 Tick(f64) override;
    b8 Terminate() override;

    void LogToFile(const char* message);
    private:
    RTFArchive* logFile;
    constexpr static const char* logFileName = "Engine.log";
    constexpr static const char* logDir = "Logs";
};
}

namespace
{
template <typename... Args>
void InnerLog(E_LOG_LEVEL level, Args&&... args)
{
    ReiToEngine::RTLoggerSystem::Instance().LogToFile(Log(level, std::forward<Args>(args)...).c_str());
}

template <typename... Args>
void InnerLogFmt(E_LOG_LEVEL level, std::format_string<Args...> fmt, Args&&... args)
{
    ReiToEngine::RTLoggerSystem::Instance().LogToFile(
    LogFmt(level, fmt, std::forward<Args>(args)...).c_str());
}
}

#define RT_LOG_FATAL(...)          InnerLog(E_LOG_LEVEL::LOG_LEVEL_FATAL __VA_OPT__(,) __VA_ARGS__)
#define RT_LOG_FATAL_FMT(fmt, ...) InnerLogFmt(E_LOG_LEVEL::LOG_LEVEL_FATAL, fmt __VA_OPT__(,) __VA_ARGS__)

#define RT_LOG_ERROR(...)          InnerLog(E_LOG_LEVEL::LOG_LEVEL_ERROR __VA_OPT__(,) __VA_ARGS__)
#define RT_LOG_ERROR_FMT(fmt, ...) InnerLogFmt(E_LOG_LEVEL::LOG_LEVEL_ERROR, fmt __VA_OPT__(,) __VA_ARGS__)

#define RT_LOG_WARN(...)           InnerLog(E_LOG_LEVEL::LOG_LEVEL_WARN __VA_OPT__(,) __VA_ARGS__)
#define RT_LOG_WARN_FMT(fmt, ...)  InnerLogFmt(E_LOG_LEVEL::LOG_LEVEL_WARN, fmt __VA_OPT__(,) __VA_ARGS__)

#define RT_LOG_INFO(...)           InnerLog(E_LOG_LEVEL::LOG_LEVEL_INFO __VA_OPT__(,) __VA_ARGS__)
#define RT_LOG_INFO_FMT(fmt, ...)  InnerLogFmt(E_LOG_LEVEL::LOG_LEVEL_INFO, fmt __VA_OPT__(,) __VA_ARGS__)

#define RT_LOG_DEBUG(...)          InnerLog(E_LOG_LEVEL::LOG_LEVEL_DEBUG __VA_OPT__(,) __VA_ARGS__)
#define RT_LOG_DEBUG_FMT(fmt, ...) InnerLogFmt(E_LOG_LEVEL::LOG_LEVEL_DEBUG, fmt __VA_OPT__(,) __VA_ARGS__)

#define RT_LOG_TRACE(...)          InnerLog(E_LOG_LEVEL::LOG_LEVEL_TRACE __VA_OPT__(,) __VA_ARGS__)
#define RT_LOG_TRACE_FMT(fmt, ...) InnerLogFmt(E_LOG_LEVEL::LOG_LEVEL_TRACE, fmt __VA_OPT__(,) __VA_ARGS__)
#endif
