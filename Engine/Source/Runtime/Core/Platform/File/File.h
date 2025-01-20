#ifndef CORE_PLATFORM_FILE_FILE_H
#define CORE_PLATFORM_FILE_FILE_H
#include "Core/HAL/Platform.h"
#include <filesystem>
namespace ReiToEngine
{
class RTENGINE_API RTFile
{
public:
	enum //Open Mode
	{
		OM_RB,
		OM_WB,
		OM_RT,
		OM_WT,
		OM_MAX
	};
	enum 
	{
		VSMAX_PATH = 256
	};
	enum	//Seek Flag
	{
		SF_CUR,
		SF_END,
		SF_SET,
		SF_MAX
	};
	RTFile();
	~RTFile();
	bool Flush();

	bool Seek(unsigned int uiOffSet,unsigned int uiOrigin);
	bool Open(const char * pFileName,unsigned int uiOpenMode);
	bool Write(const void *pBuffer,unsigned int uiSize,unsigned int uiCount);
	bool Read(void *pBuffer,unsigned int uiSize,unsigned int uiCount);
	bool GetLine(void * pBuffer,unsigned int uiSize);
	RT_FORCEINLINE uint32_t GetFileSize()const
	{
		return m_uiFileSize;
	}
	protected:
	static char ms_cOpenMode[OM_MAX][5];
	static unsigned int m_uiSeekFlag[];
	std::FILE *m_pFileHandle;
	unsigned int m_uiOpenMode;
	unsigned int m_uiFileSize;
	char m_tcFileName[VSMAX_PATH];
	};
}
#endif