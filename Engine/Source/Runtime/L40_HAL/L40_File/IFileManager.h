#ifndef HAL_FILE_IFILEMANAGER_H
#define HAL_FILE_IFILEMANAGER_H

#include <iostream>
#include "L0_Macro/Include.h"
#include "CFile.h"
#include "L20_Platform/Include.h"
namespace ReiToEngine
{
template <typename T>
class RTENGINE_API RTFileManager : public Singleton<RTFileManager<T>>
{
public:
    RTFileManager() = default;
    virtual ~RTFileManager() = 0; // 声明
	virtual void ProcessCommandLineOptions() = 0;
	virtual RTFArchive* CreateFileReader(const char* Filename, u32 ReadFlags = 0 ) = 0;
	virtual RTFArchive* CreateFileWriter( const char* Filename, u32 WriteFlags=0 )=0;
	virtual RTFArchive* CreateDebugFileWriter(const char* Filename, u32 WriteFlags=0 ) = 0;
	virtual b8 IsReadOnly( const char* Filename )=0;
	virtual b8 Delete( const char* Filename, b8 RequireExists=0, b8 EvenReadOnly=0, b8 Quiet=0 )=0;
	virtual u32 Copy( const char* Dest, const char* Src, b8 Replace=1, b8 EvenIfReadOnly=0, b8 Attributes=0, ECopyResult* Progress = nullptr, EFileRead ReadFlags=EFileRead::None, EFileWrite WriteFlags=EFileWrite::None)=0; // utility
	virtual b8 Move( const char* Dest, const char* Src, b8 Replace=1, b8 EvenIfReadOnly=0, b8 Attributes=0, b8 bDoNotRetryOrError=0 )=0;
	virtual b8 FileExists( const char* Filename )=0;
	virtual b8 DirectoryExists( const char* InDirectory )=0;
	virtual b8 MakeDirectory( const char* Path, b8 Tree=0 )=0;
	virtual b8 DeleteDirectory( const char* Path, b8 RequireExists=0, b8 Tree=0 )=0;
	virtual SFileStatData GetStatData(const char* FilenameOrDirectory) = 0;
	// virtual void FindFiles(TArray<FString>& FileNames, const char* Filename, b8 Files, b8 Directories)=0;
	// virtual void FindFiles(TArray<FString>& FoundFiles, const char* Directory, const char* FileExtension = nullptr) = 0;
	// virtual void FindFilesRecursive( TArray<FString>& FileNames, const char* StartDirectory, const char* Filename, b8 Files, b8 Directories, b8 bClearFileNames=true) = 0; // utility
	// virtual b8 IterateDirectory(const char* Directory, IPlatformFile::FDirectoryVisitor& Visitor) = 0;
	// virtual b8 IterateDirectory(const char* Directory, IPlatformFile::FDirectoryVisitorFunc Visitor) = 0;
	// virtual b8 IterateDirectoryRecursively(const char* Directory, IPlatformFile::FDirectoryVisitor& Visitor) = 0;
	// virtual b8 IterateDirectoryRecursively(const char* Directory, IPlatformFile::FDirectoryVisitorFunc Visitor) = 0;
	// virtual b8 IterateDirectoryStat(const char* Directory, IPlatformFile::FDirectoryStatVisitor& Visitor) = 0;
	// virtual b8 IterateDirectoryStat(const char* Directory, IPlatformFile::FDirectoryStatVisitorFunc Visitor) = 0;
	// virtual b8 IterateDirectoryStatRecursively(const char* Directory, IPlatformFile::FDirectoryStatVisitor& Visitor) = 0;
	// virtual b8 IterateDirectoryStatRecursively(const char* Directory, IPlatformFile::FDirectoryStatVisitorFunc Visitor) = 0;
	// virtual double GetFileAgeSeconds( const char* Filename )=0;
	// virtual FDateTime GetTimeStamp( const char* Path ) = 0;
	// virtual void GetTimeStampPair(const char* PathA, const char* PathB, FDateTime& OutTimeStampA, FDateTime& OutTimeStampB) = 0;
	// virtual b8 SetTimeStamp( const char* Path, FDateTime TimeStamp ) = 0;
	// virtual FDateTime GetAccessTimeStamp( const char* Filename ) = 0;
	// virtual FString ConvertToRelativePath( const char* Filename ) = 0;
	// virtual FString ConvertToAbsolutePathForExternalAppForRead( const char* Filename ) = 0;
	// virtual FString ConvertToAbsolutePathForExternalAppForWrite( const char* Filename ) = 0;
	virtual uint64_t FileSize( const char* Filename )=0;
	// virtual b8 SendMessageToServer(const char* Message, IPlatformFile::IFileServerMessageHandler* Handler)=0;
	// virtual FString GetFilenameOnDisk(const char* Filename) = 0;
	// static FString DefaultConvertToRelativePath( const char* Filename );
private:
	virtual RTFArchive* CreateFileReaderInternal(const char* Filename, u32 ReadFlags, u32 BufferSize) = 0;
	virtual RTFArchive* CreateFileWriterInternal(const char* Filename, u32 WriteFlags, u32 BufferSize) = 0;
	virtual u32 CopyWithProgress(const char* InDestFile, const char* InSrcFile, b8 ReplaceExisting, b8 EvenIfReadOnly, b8 Attributes, ECopyResult* Progress, EFileRead ReadFlags, EFileWrite WriteFlags) = 0;
	// virtual void FindFilesRecursiveInternal( TArray<FString>& FileNames, const char* StartDirectory, const char* Filename, b8 Files, b8 Directories) = 0;
};
}
#endif
