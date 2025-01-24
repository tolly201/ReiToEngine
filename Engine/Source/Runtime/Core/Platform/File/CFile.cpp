#include "CFile.h"

namespace ReiToEngine
{
bool RTCFile::FlushImpl()
{
	fs.flush();
	if (fs.bad()) {
		return false; 
	} else if (fs.fail()) {
		return false;
	}
	return true;
}
bool RTCFile::SeekImpl(uint32_t offset,uint32_t origin)
{
    fs.clear(); // 清除任何错误标志，这很重要！

    // 将 uint32_t 类型的 origin 转换为 std::ios_base::seekdir
    std::ios_base::seekdir seek_origin;
    switch (origin) {
        case 0:
            seek_origin = std::ios_base::beg;
            break;
        case 1:
            seek_origin = std::ios_base::cur;
            break;
        case 2:
            seek_origin = std::ios_base::end;
            break;
        default:
			return false;
    }

    fs.seekg(offset, seek_origin);
    if (!fs.good()) { // 检查 seekg 是否成功
        return false;
    }

    fs.seekp(offset, seek_origin);
    if (!fs.good()) { // 检查 seekp 是否成功
        return false;
    }
    return true;
}
bool RTCFile::OpenImpl(const char * fileName,EFileOpenFlags openMode)
{
	auto openmode = std::ios::in;
  // 使用按位与（&）来检查标志
  if (openMode & EFileOpenFlags::IO_WRITE) {
    openmode |= std::ios::out; // 添加写入模式
    if (openMode & EFileOpenFlags::IO_APPEND) {
      openmode |= std::ios::app; // 添加追加模式
    } else { // 写入但不追加，则截断文件
        openmode |= std::ios::trunc; // 除非你希望保留现有内容
    }
  	}
	fs.open(fileName, openmode);
	if (fs.bad()) {
		return false; 
	} else if (fs.fail()) {
		return false;
	}
	return true;
}
bool RTCFile::WriteImpl(const char* buffer,uint32_t size)
{
	fs.write(buffer, size);
	if (fs.bad()) {
        return false;
    } else if (fs.fail()) {
        return false;
    }
    return true; 
}
bool RTCFile::ReadImpl(char* buffer,uint32_t size)
{
	fs.read(buffer, size);
	if (fs.bad()) {
        return false;
    } else if (fs.fail()) {
        return false;
    }
    return fs.gcount() == size;
}
bool RTCFile::GetLineImpl(char* buffer,uint32_t size)
{
	fs.getline(buffer, size);
	if (fs.bad()) {
        return false;
    } else if (fs.fail()) {
        return false;
    }
    return fs.gcount() == size;
}
bool RTCFile::CloseImpl()
{
	fs.close();
	return !fs.fail();
}
}