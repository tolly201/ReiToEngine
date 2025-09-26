#ifndef HAL_FILE__CFILE_H
#define HAL_FILE__CFILE_H
#include "IFile.h"
#include <fstream>
namespace ReiToEngine
{
class RTENGINE_API RTCFile : public RTFile<RTCFile>
{
public:
	RTCFile();
	~RTCFile();
	b8 FlushImpl() override;
	b8 SeekImpl(u32 offset,u32 origin) override;
	b8 OpenImpl(const char* fileName,EFileOpenFlags openMode) override;
	b8 WriteImpl(const char* buffer,u32 size) override;
	b8 ReadImpl(char* buffer,u32 size) override;
	b8 GetLineImpl(char* buffer,u32 size) override;
	b8 CloseImpl() override;
protected:
	std::fstream fs;
};

class RTENGINE_API RTFArchive : public RTCFile
{};
}
#endif
