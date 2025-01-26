#ifndef CORE_COMMON_FILEMANAGER_CFILEMANAGER_H
#define CORE_COMMON_FILEMANAGER_CFILEMANAGER_H

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
	RTFArchive* CreateFileReader(const char* Filename, uint32_t ReadFlags = 0 ) override {}
	RTFArchive* CreateFileWriter( const char* Filename, uint32_t WriteFlags=0 ) override {};
	RTFArchive* CreateDebugFileWriter(const char* Filename, uint32_t WriteFlags=0 ) override {
        RTFArchive* ret = new RTFArchive();
        ret->Open(Filename, EFileOpenFlags::IO_READ | EFileOpenFlags::IO_WRITE);
        return ret;
    }
	bool IsReadOnly( const char* Filename ) override {}
	bool Delete( const char* Filename, bool RequireExists=0, bool EvenReadOnly=0, bool Quiet=0 ) override {}
	uint32_t Copy( const char* Dest, const char* Src, bool Replace=1, bool EvenIfReadOnly=0, bool Attributes=0, ECopyResult* Progress = nullptr, EFileRead ReadFlags=FILEREAD_None, EFileWrite WriteFlags=FILEWRITE_None) override {}
	bool Move( const char* Dest, const char* Src, bool Replace=1, bool EvenIfReadOnly=0, bool Attributes=0, bool bDoNotRetryOrError=0 ) override {}
	bool FileExists( const char* Filename ) override {}
	bool DirectoryExists( const char* InDirectory )override {}
	bool MakeDirectory( const char* Path, bool Tree=0 )override {}
	bool DeleteDirectory( const char* Path, bool RequireExists=0, bool Tree=0 )override {}
	SFileStatData GetStatData(const char* FilenameOrDirectory) override {}
	// void FindFiles(TArray<FString>& FileNames, const char* Filename, bool Files, bool Directories)override {}
	// void FindFiles(TArray<FString>& FoundFiles, const char* Directory, const char* FileExtension = nullptr) override {}
	// void FindFilesRecursive( TArray<FString>& FileNames, const char* StartDirectory, const char* Filename, bool Files, bool Directories, bool bClearFileNames=true) override {} // utility
	// bool IterateDirectory(const char* Directory, IPlatformFile::FDirectoryVisitor& Visitor) override {}
	// bool IterateDirectory(const char* Directory, IPlatformFile::FDirectoryVisitorFunc Visitor) override {}
	// bool IterateDirectoryRecursively(const char* Directory, IPlatformFile::FDirectoryVisitor& Visitor) override {}
	// bool IterateDirectoryRecursively(const char* Directory, IPlatformFile::FDirectoryVisitorFunc Visitor) override {}
	// bool IterateDirectoryStat(const char* Directory, IPlatformFile::FDirectoryStatVisitor& Visitor) override {}
	// bool IterateDirectoryStat(const char* Directory, IPlatformFile::FDirectoryStatVisitorFunc Visitor) override {}
	// bool IterateDirectoryStatRecursively(const char* Directory, IPlatformFile::FDirectoryStatVisitor& Visitor) override {}
	// bool IterateDirectoryStatRecursively(const char* Directory, IPlatformFile::FDirectoryStatVisitorFunc Visitor) override {}
	// double GetFileAgeSeconds( const char* Filename )override {}
	// FDateTime GetTimeStamp( const char* Path ) override {}
	// void GetTimeStampPair(const char* PathA, const char* PathB, FDateTime& OutTimeStampA, FDateTime& OutTimeStampB) override {}
	// bool SetTimeStamp( const char* Path, FDateTime TimeStamp ) override {}
	// FDateTime GetAccessTimeStamp( const char* Filename ) override {}
	// FString ConvertToRelativePath( const char* Filename ) override {}
	// FString ConvertToAbsolutePathForExternalAppForRead( const char* Filename ) override {}
	// FString ConvertToAbsolutePathForExternalAppForWrite( const char* Filename ) override {}
	uint64_t FileSize( const char* Filename )override {}
	// bool SendMessageToServer(const char* Message, IPlatformFile::IFileServerMessageHandler* Handler)override {}
	// FString GetFilenameOnDisk(const char* Filename) override {}
	// static FString DefaultConvertToRelativePath( const char* Filename );
private:
	RTFArchive* CreateFileReaderInternal(const char* Filename, uint32_t ReadFlags, uint32_t BufferSize) override {}
	RTFArchive* CreateFileWriterInternal(const char* Filename, uint32_t WriteFlags, uint32_t BufferSize) override {}
	uint32_t CopyWithProgress(const char* InDestFile, const char* InSrcFile, bool ReplaceExisting, bool EvenIfReadOnly, bool Attributes, ECopyResult* Progress, EFileRead ReadFlags, EFileWrite WriteFlags) override {}
	// void FindFilesRecursiveInternal( TArray<FString>& FileNames, const char* StartDirectory, const char* Filename, bool Files, bool Directories) override {}
};
template <typename T>
RTFileManager<T>::~RTFileManager() = default;
}

#endif
