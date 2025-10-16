#ifndef HAL_FILE_IFILE_H
#define HAL_FILE_IFILE_H
#include "L0_Macro/Include.h"
#include "FileDefines.h"
namespace ReiToEngine
{
template <typename T>
class RTFile
{
public:
	RTFile() = default;
	virtual ~RTFile() = 0;
	b8 Flush()
	{
		return static_cast<T*>(this)->FlushImpl();
	}
	b8 Seek(u32 offset,u32 origin)
	{
		return static_cast<T*>(this)->SeekImpl(offset, origin);
	}
	b8 Open(const char* fileName, EFileOpenFlags openMode)
	{
		return static_cast<T*>(this)->OpenImpl(fileName, openMode);
	}
	b8 Write(const char* buffer,u32 size)
	{
		return static_cast<T*>(this)->WriteImpl(buffer, size);
	}
	b8 Read(char* buffer, u32 size)
	{
		return static_cast<T*>(this)->ReadImpl(buffer, size);
	}
	b8 GetLine(char* buffer, u32 size)
	{
		return static_cast<T*>(this)->GetLineImpl(buffer, size);
	}
	b8 Close()
	{
		return static_cast<T*>(this)->CloseImpl();
	}
	RT_FORCEINLINE u32 GetFileSize()const
	{
		return FileSize;
	}
    RT_FORCEINLINE void* GetBuffer() {
        return static_cast<T*>(this)->GetBufferImpl();
    }

    b8 IsValid() const;
protected:
	virtual b8 FlushImpl() = 0;
	virtual b8 SeekImpl(u32 offset,u32 origin) = 0;
	virtual b8 OpenImpl(const char* fileName,EFileOpenFlags openMode) = 0;
	virtual b8 WriteImpl(const char* buffer,u32 size) = 0;
	virtual b8 ReadImpl(char* buffer,u32 size) = 0;
	virtual b8 GetLineImpl(char* buffer,u32 size) = 0;
	virtual b8 CloseImpl() = 0;
    virtual void* GetBufferImpl() = 0;
	u32 FileSize;
	char* FilePath = new char[static_cast<u32>(EFileCommon::FILE_MAX_PATH)];
	SFileStatData FileState;
	};
template <typename T>
RTFile<T>::~RTFile() = default;
}
#endif
