#ifndef HAL_FILE_CFILEMANAGER_H
#define HAL_FILE_CFILEMANAGER_H

#include "IFileManager.h"
#include <filesystem>
#include <optional>
#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif
#ifdef _WIN32
#   include <windows.h>
#   include <vector>
#endif
namespace{
inline std::optional<std::filesystem::path> GetExecutableDir() {
    namespace fs = std::filesystem;
#ifdef __APPLE__
    uint32_t size = 0;
    _NSGetExecutablePath(nullptr, &size);
    if (size == 0) return std::nullopt;
    std::vector<char> buf(size);
    if (_NSGetExecutablePath(buf.data(), &size) != 0) {
        return std::nullopt;
    }
    try {
        fs::path p = fs::weakly_canonical(fs::path(buf.data()));
        if (p.has_filename()) p = p.parent_path();
        return p;
    } catch (...) {
        return std::nullopt;
    }
#elif defined(_WIN32)
    // Windows: 使用 GetModuleFileNameW (宽字符) 取得当前模块路径，然后取其父目录
    std::vector<wchar_t> buffer(512);
    DWORD len = 0;
    while (true) {
        len = GetModuleFileNameW(nullptr, buffer.data(), static_cast<DWORD>(buffer.size()));
        RT_LOG_DEBUG_FMT_PLATFORM("GetModuleFileNameW returned len = {}", len);
        if (len == 0) {
            RT_LOG_ERROR_PLATFORM("GetModuleFileNameW failed.");
            return std::nullopt; // 失败
        }
        // 若返回长度小于缓冲区 - 1，则表示完整；否则需要扩容重试
        if (len < buffer.size() - 1) {
            break;
        }
        buffer.resize(buffer.size() * 2);
        if (buffer.size() > 1 << 15) { // 防止无限扩张 (~32K chars 足够)
            return std::nullopt;
        }
    }
    try {
        fs::path p(buffer.data());
        // 规整路径 (weakly_canonical 避免软链接/相对跳转)
        try { p = fs::weakly_canonical(p); } catch (...) {}
        if (p.has_filename()) p = p.parent_path();

        RT_LOG_DEBUG_FMT_PLATFORM("Executable path: {}", p.string().c_str());
        return p;
    } catch (...) {
        return std::nullopt;
    }
#else
    // 其它平台尚未实现。可后续用 /proc/self/exe (Linux) 或 argv[0] 结合 realpath 实现。
    return std::nullopt;
#endif
}

inline std::optional<std::filesystem::path> FindRootDir(const std::string& marker = "ReiToEngine") {
    namespace fs = std::filesystem;
    static std::optional<fs::path> cached;
    if (cached) return cached;

    // 1. 优先使用可执行文件目录
    fs::path start;
    if (auto execDir = GetExecutableDir()) {
        start = *execDir;
    } else {
        // 2. 回退当前工作目录
        try {
            start = fs::current_path();
        } catch (...) {
            return std::nullopt;
        }
    }

    // 3. 向上查找
    for (fs::path p = start; !p.empty(); p = p.parent_path()) {
        if (p.filename() == marker) {
            try {
                cached = fs::weakly_canonical(p);
            } catch (...) {
                cached = p;
            }
            return cached;
        }
    }

    // 4. 附加搜索：在若干上层目录里“一层子目录扫描”（防止运行于 builds/xxx 目录深处）
    fs::path probe = start;
    for (int i = 0; i < 4 && !probe.empty(); ++i) {
        try {
            for (auto& entry : fs::directory_iterator(probe)) {
                if (entry.is_directory() && entry.path().filename() == marker) {
                    try {
                        cached = fs::weakly_canonical(entry.path());
                    } catch (...) {
                        cached = entry.path();
                    }
                    return cached;
                }
            }
        } catch (...) { /* 忽略权限错误 */ }
        probe = probe.parent_path();
    }

    return std::nullopt;
}
}
namespace ReiToEngine
{
class RTENGINE_API RTCFileManager : public RTFileManager<RTCFileManager>
{
    friend class Runtime_Singleton<RTCFileManager>;
    friend class RTFileManager<RTCFileManager>;
    friend class RTFileManager;
public:
    RTCFileManager() = default;
    ~RTCFileManager() = default;
    b8 Initialize() override {
        RootDir = String(
        FindRootDir().has_value() ? FindRootDir().value().string() : "");
        RT_LOG_INFO_FMT_PLATFORM("RootDir: {}", RootDir.c_str());
        return true;
    };
    b8 Tick([[maybe_unused]]f64) override {return true;};
    b8 Terminate() override {return true;};
	void ProcessCommandLineOptions() override {}
	RTFArchive* CreateFileReader([[maybe_unused]]const char* Filename,[[maybe_unused]] u32 ReadFlags = 0) override { return nullptr; }
	RTFArchive* CreateFileWriter([[maybe_unused]] const char* Filename,[[maybe_unused]] u32 WriteFlags=0 ) override { return nullptr; };
	RTFArchive* CreateDebugFileWriter(const char* Filename,[[maybe_unused]] u32 WriteFlags=0 ) override {
        String file_path = RootDir + "/" + Filename;
        RTFArchive* ret = new RTFArchive();
        ret->Open(file_path.c_str(), EFileOpenFlags::IO_READ | EFileOpenFlags::IO_WRITE);
        return ret;
    }
	b8 IsReadOnly([[maybe_unused]] const char* Filename ) override { return false; }
	b8 Delete([[maybe_unused]] const char* Filename,[[maybe_unused]] b8 RequireExists=0,[[maybe_unused]] b8 EvenReadOnly=0,[[maybe_unused]] b8 Quiet=0 ) override { return false; }
	u32 Copy([[maybe_unused]] const char* Dest,[[maybe_unused]] const char* Src,[[maybe_unused]] b8 Replace=1,[[maybe_unused]] b8 EvenIfReadOnly=0,[[maybe_unused]] b8 Attributes=0,[[maybe_unused]] ECopyResult* Progress = nullptr,[[maybe_unused]] EFileRead ReadFlags=EFileRead::None,[[maybe_unused]] EFileWrite WriteFlags=EFileWrite::None) override { return false; }
	b8 Move([[maybe_unused]] const char* Dest,[[maybe_unused]] const char* Src,[[maybe_unused]] b8 Replace=1,[[maybe_unused]] b8 EvenIfReadOnly=0,[[maybe_unused]] b8 Attributes=0,[[maybe_unused]] b8 bDoNotRetryOrError=0 ) override { return false; }
	b8 FileExists(const char* Filename ) override;
	b8 DirectoryExists(const char* InDirectory )override;
	b8 MakeDirectory(const char* Path, b8 Tree=0 )override;
	b8 DeleteDirectory([[maybe_unused]] const char* Path,
    [[maybe_unused]] b8 RequireExists=0,[[maybe_unused]] b8 Tree=0 )override { return false; }
	b8 TouchFile(const char* Filename);

    SFileStatData GetStatData([[maybe_unused]]const char* FilenameOrDirectory) override { return SFileStatData(); }
	// void FindFiles(TArray<FString>& FileNames, const char* Filename, b8 Files, b8 Directories)override {}
	// void FindFiles(TArray<FString>& FoundFiles, const char* Directory, const char* FileExtension = nullptr) override {}
	// void FindFilesRecursive( TArray<FString>& FileNames, const char* StartDirectory, const char* Filename, b8 Files, b8 Directories, b8 bClearFileNames=true) override {} // utility
	// b8 IterateDirectory(const char* Directory, IPlatformFile::FDirectoryVisitor& Visitor) override {}
	// b8 IterateDirectory(const char* Directory, IPlatformFile::FDirectoryVisitorFunc Visitor) override {}
	// b8 IterateDirectoryRecursively(const char* Directory, IPlatformFile::FDirectoryVisitor& Visitor) override {}
	// b8 IterateDirectoryRecursively(const char* Directory, IPlatformFile::FDirectoryVisitorFunc Visitor) override {}
	// b8 IterateDirectoryStat(const char* Directory, IPlatformFile::FDirectoryStatVisitor& Visitor) override {}
	// b8 IterateDirectoryStat(const char* Directory, IPlatformFile::FDirectoryStatVisitorFunc Visitor) override {}
	// b8 IterateDirectoryStatRecursively(const char* Directory, IPlatformFile::FDirectoryStatVisitor& Visitor) override {}
	// b8 IterateDirectoryStatRecursively(const char* Directory, IPlatformFile::FDirectoryStatVisitorFunc Visitor) override {}
	// double GetFileAgeSeconds( const char* Filename )override {}
	// FDateTime GetTimeStamp( const char* Path ) override {}
	// void GetTimeStampPair(const char* PathA, const char* PathB, FDateTime& OutTimeStampA, FDateTime& OutTimeStampB) override {}
	// b8 SetTimeStamp( const char* Path, FDateTime TimeStamp ) override {}
	// FDateTime GetAccessTimeStamp( const char* Filename ) override {}
	// FString ConvertToRelativePath( const char* Filename ) override {}
	// FString ConvertToAbsolutePathForExternalAppForRead( const char* Filename ) override {}
	// FString ConvertToAbsolutePathForExternalAppForWrite( const char* Filename ) override {}
	uint64_t FileSize([[maybe_unused]]const char* Filename)override { return 0; }
	// b8 SendMessageToServer(const char* Message, IPlatformFile::IFileServerMessageHandler* Handler)override {}
	// FString GetFilenameOnDisk(const char* Filename) override {}
	// static FString DefaultConvertToRelativePath( const char* Filename );
private:
	RTFArchive* CreateFileReaderInternal([[maybe_unused]]const char* Filename,[[maybe_unused]] u32 ReadFlags,[[maybe_unused]] u32 BufferSize) override { return nullptr; }
	RTFArchive* CreateFileWriterInternal([[maybe_unused]]const char* Filename,[[maybe_unused]] u32 WriteFlags,[[maybe_unused]] u32 BufferSize) override { return nullptr; }
	u32 CopyWithProgress([[maybe_unused]]const char* InDestFile,[[maybe_unused]] const char* InSrcFile,[[maybe_unused]] b8 ReplaceExisting,[[maybe_unused]] b8 EvenIfReadOnly,[[maybe_unused]] b8 Attributes, [[maybe_unused]]ECopyResult* Progress,[[maybe_unused]] EFileRead ReadFlags,[[maybe_unused]] EFileWrite WriteFlags) override { return 0; }
	// void FindFilesRecursiveInternal( TArray<FString>& FileNames, const char* StartDirectory, const char* Filename, b8 Files, b8 Directories) override {}
};
}

#endif
