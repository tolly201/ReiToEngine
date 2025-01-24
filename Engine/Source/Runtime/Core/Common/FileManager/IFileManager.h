#ifndef CORE_COMMON_FILEMANAGER_IFILEMANAGER_H
#define CORE_COMMON_FILEMANAGER_IFILEMANAGER_H

#include <iostream>
#include "Core/HAL/Platform.h"
#include "Core/Platform/File/IFile.h"
namespace ReiToEngine
{
template <typename T>
class RTENGINE_API RTFileManager
{
public:
    RTFileManager() = default;
    virtual ~RTFileManager() = 0; // 声明
	virtual void ProcessCommandLineOptions() = 0;
	virtual RTFArchive* CreateFileReader(const char* Filename, uint32_t ReadFlags = 0 ) = 0;
	virtual RTFArchive* CreateFileWriter( const char* Filename, uint32_t WriteFlags=0 )=0;
	virtual RTFArchive* CreateDebugFileWriter(const char* Filename, uint32_t WriteFlags=0 ) = 0;
	virtual bool IsReadOnly( const char* Filename )=0;
	virtual bool Delete( const char* Filename, bool RequireExists=0, bool EvenReadOnly=0, bool Quiet=0 )=0;
	virtual uint32_t Copy( const char* Dest, const char* Src, bool Replace=1, bool EvenIfReadOnly=0, bool Attributes=0, FCopyProgress* Progress = nullptr, EFileRead ReadFlags=FILEREAD_None, EFileWrite WriteFlags=FILEWRITE_None)=0; // utility
	virtual bool Move( const char* Dest, const char* Src, bool Replace=1, bool EvenIfReadOnly=0, bool Attributes=0, bool bDoNotRetryOrError=0 )=0;
	virtual bool FileExists( const char* Filename )=0;
	virtual bool DirectoryExists( const char* InDirectory )=0;
	virtual bool MakeDirectory( const char* Path, bool Tree=0 )=0;
	virtual bool DeleteDirectory( const char* Path, bool RequireExists=0, bool Tree=0 )=0;
	virtual SFileStatData GetStatData(const char* FilenameOrDirectory) = 0;
	virtual void FindFiles(TArray<FString>& FileNames, const char* Filename, bool Files, bool Directories)=0;
	virtual void FindFiles(TArray<FString>& FoundFiles, const char* Directory, const char* FileExtension = nullptr) = 0;
	virtual void FindFilesRecursive( TArray<FString>& FileNames, const char* StartDirectory, const char* Filename, bool Files, bool Directories, bool bClearFileNames=true) = 0; // utility
	virtual bool IterateDirectory(const char* Directory, IPlatformFile::FDirectoryVisitor& Visitor) = 0;
	virtual bool IterateDirectory(const char* Directory, IPlatformFile::FDirectoryVisitorFunc Visitor) = 0;
	virtual bool IterateDirectoryRecursively(const char* Directory, IPlatformFile::FDirectoryVisitor& Visitor) = 0;
	virtual bool IterateDirectoryRecursively(const char* Directory, IPlatformFile::FDirectoryVisitorFunc Visitor) = 0;
	virtual bool IterateDirectoryStat(const char* Directory, IPlatformFile::FDirectoryStatVisitor& Visitor) = 0;
	virtual bool IterateDirectoryStat(const char* Directory, IPlatformFile::FDirectoryStatVisitorFunc Visitor) = 0;
	virtual bool IterateDirectoryStatRecursively(const char* Directory, IPlatformFile::FDirectoryStatVisitor& Visitor) = 0;
	virtual bool IterateDirectoryStatRecursively(const char* Directory, IPlatformFile::FDirectoryStatVisitorFunc Visitor) = 0;
	virtual double GetFileAgeSeconds( const char* Filename )=0;
	virtual FDateTime GetTimeStamp( const char* Path ) = 0;
	virtual void GetTimeStampPair(const char* PathA, const char* PathB, FDateTime& OutTimeStampA, FDateTime& OutTimeStampB) = 0;
	virtual bool SetTimeStamp( const char* Path, FDateTime TimeStamp ) = 0;
	virtual FDateTime GetAccessTimeStamp( const char* Filename ) = 0;
	virtual FString ConvertToRelativePath( const char* Filename ) = 0;
	virtual FString ConvertToAbsolutePathForExternalAppForRead( const char* Filename ) = 0;
	virtual FString ConvertToAbsolutePathForExternalAppForWrite( const char* Filename ) = 0;
	virtual int64 FileSize( const char* Filename )=0;
	virtual bool SendMessageToServer(const char* Message, IPlatformFile::IFileServerMessageHandler* Handler)=0;
	virtual FString GetFilenameOnDisk(const char* Filename) = 0;
	static FString DefaultConvertToRelativePath( const char* Filename );
private:
	virtual RTFArchive * CreateFileReaderInternal(const char* Filename, uint32_t ReadFlags, uint32_t BufferSize) = 0;
	virtual RTFArchive* CreateFileWriterInternal(const char* Filename, uint32_t WriteFlags, uint32_t BufferSize) = 0;
	virtual uint32_t CopyWithProgress(const char* InDestFile, const char* InSrcFile, bool ReplaceExisting, bool EvenIfReadOnly, bool Attributes, FCopyProgress* Progress, EFileRead ReadFlags, EFileWrite WriteFlags) = 0;
	virtual void FindFilesRecursiveInternal( TArray<FString>& FileNames, const char* StartDirectory, const char* Filename, bool Files, bool Directories) = 0;
};
}
#endif
