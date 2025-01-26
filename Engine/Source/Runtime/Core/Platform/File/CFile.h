#ifndef CORE_PLATFORM_FILE_CFILE_H
#define CORE_PLATFORM_FILE_CFILE_H
#include "IFile.h"
#include <fstream>
namespace ReiToEngine
{
class RTENGINE_API RTCFile : public RTFile<RTCFile>
{
public:
	RTCFile();
	~RTCFile();
	bool FlushImpl() override;
	bool SeekImpl(uint32_t offset,uint32_t origin) override;
	bool OpenImpl(const char* fileName,uint32_t openMode) override;
	bool WriteImpl(const char* buffer,uint32_t size) override;
	bool ReadImpl(char* buffer,uint32_t size) override;
	bool GetLineImpl(char* buffer,uint32_t size) override;
	bool CloseImpl() override;
protected:
	std::fstream fs;
};

class RTENGINE_API RTFArchive : public RTCFile
{}; 
}
#endif