#ifndef CORE_PLATFORM_FILE_CFILE_H
#define CORE_PLATFORM_FILE_CFILE_H
#include "IFile.h"
#include <fstream>
namespace ReiToEngine

{
class RTENGINE_API RTCFile : public RTFile<RTCFile>
{
public:
	RTCFile() = default;
	~RTCFile() = default;
	bool FlushImpl() override;
	bool SeekImpl(uint32_t offset,uint32_t origin) override;
	bool OpenImpl(const char* fileName,EFileOpenFlags openMode) override;
	bool WriteImpl(const char* buffer,uint32_t size) override;
	bool ReadImpl(char* buffer,uint32_t size) override;
	bool GetLineImpl(char* buffer,uint32_t size) override;
	bool CloseImpl() override;
protected:
	std::fstream fs;
};
}
#endif