#ifndef CORE_PLATFORM_FILE_IFILE_H
#define CORE_PLATFORM_FILE_IFILE_H
#include "Core/HAL/Platform.h"
namespace ReiToEngine
{
enum EFileWrite
{
	FILEWRITE_None				= 0x00,
	FILEWRITE_NoFail            = 0x01,
	FILEWRITE_NoReplaceExisting = 0x02,
	FILEWRITE_EvenIfReadOnly    = 0x04,
	FILEWRITE_Append			= 0x08,
	FILEWRITE_AllowRead			= 0x10,
	FILEWRITE_Silent			= 0x20
};

enum EFileRead
{
	FILEREAD_None				= 0x00,
	FILEREAD_NoFail             = 0x01,
	FILEREAD_Silent				= 0x02,
	FILEREAD_AllowWrite			= 0x04,
};

enum ECopyResult
{
	COPY_OK						= 0x00,
	COPY_Fail					= 0x01,
	COPY_Canceled				= 0x02,
};
enum EFileOpenFlags
{
	IO_READ			= 0x01,					// Open for reading
	IO_WRITE		= 0x02,					// Open for writing
	IO_APPEND		= 0x40,					// When writing, keep the existing data, set the filepointer to the end of the existing data
};

enum EFileCommon
{
	FILE_MAX_PATH = 256
};

struct SFileStatData
{
	EFileWrite Write;
	EFileRead Read;
	ECopyResult CopyResult;
	EFileOpenFlags OpenState;
};

template <typename T>
class RTENGINE_API RTFile
{
public:
	RTFile() = default;
	virtual ~RTFile() = 0;
	bool Flush()
	{
		return static_cast<T*>(this)->FlushImpl();
	}
	bool Seek(uint32_t offset,uint32_t origin)
	{
		return static_cast<T*>(this)->SeekImpl(offset, origin);
	}
	bool Open(const char* fileName, uint32_t openMode)
	{
		return static_cast<T*>(this)->OpenImpl(fileName, openMode);
	}
	bool Write(const char* buffer,uint32_t size)
	{
		return static_cast<T*>(this)->WriteImpl(buffer, size);
	}
	bool Read(char* buffer, uint32_t size)
	{
		return static_cast<T*>(this)->ReadImpl(buffer, size);
	}
	bool GetLine(char* buffer, uint32_t size)
	{
		return static_cast<T*>(this)->GetLineImpl(buffer, size);
	}
	bool Close()
	{
		return static_cast<T*>(this)->CloseImpl();
	}
	RT_FORCEINLINE uint32_t GetFileSize()const
	{
		return FileSize;
	}
protected:
	virtual bool FlushImpl() = 0;
	virtual bool SeekImpl(uint32_t offset,uint32_t origin) = 0;
	virtual bool OpenImpl(const char* fileName,uint32_t openMode) = 0;
	virtual bool WriteImpl(const char* buffer,uint32_t size) = 0;
	virtual bool ReadImpl(char* buffer,uint32_t size) = 0;
	virtual bool GetLineImpl(char* buffer,uint32_t size) = 0;
	virtual bool CloseImpl() = 0;
	uint32_t FileSize;
	char* FilePath = new char[EFileCommon::FILE_MAX_PATH];
	SFileStatData FileState;
	};
template <typename T>
RTFile<T>::~RTFile()
{
    Close();
};
}
#endif
