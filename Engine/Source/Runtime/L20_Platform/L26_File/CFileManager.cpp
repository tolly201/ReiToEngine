#include "CFileManager.h"

namespace ReiToEngine{
b8 RTCFileManager::DirectoryExists(const char* directory)
{
    RT_LOG_INFO_PLATFORM("Check directory exists");
    String dir = RootDir + "/" + String(directory);
    return std::filesystem::is_directory(std::filesystem::path(dir.c_str())) && std::filesystem::exists(std::filesystem::path(dir.c_str()));
}

b8 RTCFileManager::FileExists(const char* filename)
{
    RT_LOG_INFO_PLATFORM("Check directory FileExists");
    String dir = RootDir + "/" + String(filename);
    return std::filesystem::is_regular_file(std::filesystem::path(dir.c_str())) && std::filesystem::exists(std::filesystem::path(dir.c_str()));
}

b8 RTCFileManager::MakeDirectory(const char* Path, b8 Tree)
{
    RT_LOG_INFO_PLATFORM("Check directory MakeDirectory");
    String dir = RootDir + "/" + String(Path);
    if (Tree)
    {
        return std::filesystem::create_directories(std::filesystem::path(dir.c_str()));
    }
    else
    {
        return std::filesystem::create_directory(std::filesystem::path(dir.c_str()));
    }
}

b8 RTCFileManager::TouchFile(const char* Filename)
{
    RT_LOG_INFO_PLATFORM("Touch file");
    String file_path = RootDir + "/" + String(Filename);
    std::ofstream ofs(file_path.c_str(), std::ios::app);
    if (!ofs)
    {
        return false;
    }
    ofs.close();
    return true;
}
}
