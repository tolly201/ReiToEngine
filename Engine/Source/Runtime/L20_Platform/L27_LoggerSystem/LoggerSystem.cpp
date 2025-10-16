#include "LoggerSystem.h"
namespace ReiToEngine
{
b8 RTLoggerSystem::Initialize()
{
    RT_LOG_DEBUG_FMT_PLATFORM("Enter Logger Initialize");
    if (!RTCFileManager::Instance().DirectoryExists(logDir))
    {
        RTCFileManager::Instance().MakeDirectory(logDir, true);
    }

    String fullLogPath = String(logDir) + "/" + String(logFileName);

    if (!RTCFileManager::Instance().FileExists(fullLogPath.c_str()))
    {
        RTCFileManager::Instance().TouchFile(fullLogPath.c_str());
    }

    logFile = RTCFileManager::Instance().CreateDebugFileWriter(fullLogPath.c_str());
    return true;
}

b8 RTLoggerSystem::Tick(f64)
{
    return true;
}

b8 RTLoggerSystem::Terminate()
{
    logFile->Close();
    return true;
}

void RTLoggerSystem::LogToFile(const char* message)
{
    String messageString(message);
    if (!logFile)
    {
        logFile = RTCFileManager::Instance().CreateDebugFileWriter(logFileName);
    }

    if (!logFile->IsValid())
    {
        logFile->Open(logFileName, EFileOpenFlags::IO_WRITE);
    }

    logFile->Seek(0, std::ios::end);

    logFile->Write(message, static_cast<u32>(std::strlen(message)));
    logFile->Write("\n", 1);
    logFile->Flush();
}
} // namespace ReiToEngine
