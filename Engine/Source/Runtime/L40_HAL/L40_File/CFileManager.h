#ifndef HAL_FILE_CFILEMANAGER_H
#define HAL_FILE_CFILEMANAGER_H

#include "IFileManager.h"
namespace ReiToEngine
{
class RTENGINE_API RTCFileManager : public RTFileManager<RTCFileManager>
{
    friend class RTFileManager<RTCFileManager>;
public:
    RTCFileManager() = default;
    ~RTCFileManager() = default;
	void ProcessCommandLineOptions() override {}
	RTFArchive* CreateFileReader([[maybe_unused]]const char* Filename,[[maybe_unused]] u32 ReadFlags = 0) override { return nullptr; }
	RTFArchive* CreateFileWriter([[maybe_unused]] const char* Filename,[[maybe_unused]] u32 WriteFlags=0 ) override { return nullptr; };
	RTFArchive* CreateDebugFileWriter(const char* Filename,[[maybe_unused]] u32 WriteFlags=0 ) override {
        RTFArchive* ret = new RTFArchive();
        ret->Open(Filename, EFileOpenFlags::IO_READ | EFileOpenFlags::IO_WRITE);
        return ret;
    }
	b8 IsReadOnly([[maybe_unused]] const char* Filename ) override { return false; }
	b8 Delete([[maybe_unused]] const char* Filename,[[maybe_unused]] b8 RequireExists=0,[[maybe_unused]] b8 EvenReadOnly=0,[[maybe_unused]] b8 Quiet=0 ) override { return false; }
	u32 Copy([[maybe_unused]] const char* Dest,[[maybe_unused]] const char* Src,[[maybe_unused]] b8 Replace=1,[[maybe_unused]] b8 EvenIfReadOnly=0,[[maybe_unused]] b8 Attributes=0,[[maybe_unused]] ECopyResult* Progress = nullptr,[[maybe_unused]] EFileRead ReadFlags=EFileRead::None,[[maybe_unused]] EFileWrite WriteFlags=EFileWrite::None) override { return false; }
	b8 Move([[maybe_unused]] const char* Dest,[[maybe_unused]] const char* Src,[[maybe_unused]] b8 Replace=1,[[maybe_unused]] b8 EvenIfReadOnly=0,[[maybe_unused]] b8 Attributes=0,[[maybe_unused]] b8 bDoNotRetryOrError=0 ) override { return false; }
	b8 FileExists([[maybe_unused]] const char* Filename ) override { return false; }
	b8 DirectoryExists([[maybe_unused]] const char* InDirectory )override { return false; }
	b8 MakeDirectory([[maybe_unused]] const char* Path,[[maybe_unused]] b8 Tree=0 )override { return false; }
	b8 DeleteDirectory([[maybe_unused]] const char* Path,[[maybe_unused]] b8 RequireExists=0,[[maybe_unused]] b8 Tree=0 )override { return false; }
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
template <typename T>
RTFileManager<T>::~RTFileManager() = default;
}

#endif
