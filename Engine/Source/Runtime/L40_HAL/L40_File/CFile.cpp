#include "CFile.h"
#include <iostream>
#include <cstring>
namespace ReiToEngine
{
	RTCFile::RTCFile() = default;
	RTCFile::~RTCFile() {
    if (m_cachedBuffer) {
        delete[] m_cachedBuffer;
        m_cachedBuffer = nullptr;
        m_cachedBufferSize = 0;
    }
    };

b8 RTCFile::FlushImpl()
{
	fs.flush();
	if (fs.bad()) {
		return false;
	} else if (fs.fail()) {
		return false;
	}
	return true;
}
b8 RTCFile::SeekImpl(u32 offset,u32 origin)
{
    fs.clear(); // 清除任何错误标志，这很重要！

    // 将 u32 类型的 origin 转换为 std::ios_base::seekdir
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
    std::cout << "seekg fail";
        return false;
    }

    fs.seekp(offset, seek_origin);
    if (!fs.good()) { // 检查 seekp 是否成功
    std::cout << "seekp fail";
        return false;
    }
    return true;
}
b8 RTCFile::OpenImpl(const char * fileName,EFileOpenFlags openMode)
{
    auto openmode = std::ios::in;
    if ((openMode & EFileOpenFlags::IO_WRITE) == EFileOpenFlags::IO_WRITE) {
        openmode = std::ios::in;
    }

    if ((openMode & EFileOpenFlags::IO_APPEND) == EFileOpenFlags::IO_APPEND) {
        openmode |= std::ios::app;
    }

    if ((openMode & EFileOpenFlags::IO_READ) == EFileOpenFlags::IO_READ && (openMode & EFileOpenFlags::IO_WRITE) == EFileOpenFlags::IO_WRITE) {
        openmode |= std::ios::out; // 同时读写模式
    }

    std::cout <<"openmode: "<< openmode << std::endl;
	fs.open(fileName, openmode);
	if (fs.bad()) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl; // 输出错误原因
		return false;
	} else if (fs.fail()) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl; // 输出错误原因
		return false;
	}
	return true;
}
b8 RTCFile::WriteImpl(const char* buffer,u32 size)
{
	fs.write(buffer, size);
	if (fs.bad()) {
        std::cerr << "Failed to write file: " << std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl; // 输出错误原因
		return false;
	} else if (fs.fail()) {
        std::cerr << "Failed to write file: " <<std::endl;
        std::cerr << "Error: " << strerror(errno) << std::endl; // 输出错误原因
		return false;
	}
	return true;
}
b8 RTCFile::ReadImpl(char* buffer,u32 size)
{
	fs.read(buffer, size);
	if (fs.bad()) {
        return false;
    } else if (fs.fail()) {
        return false;
    }
    return fs.gcount() == size;
}
b8 RTCFile::GetLineImpl(char* buffer,u32 size)
{
	fs.getline(buffer, size);
	if (fs.bad()) {
        return false;
    } else if (fs.fail()) {
        return false;
    }
    return fs.gcount() == size;
}
b8 RTCFile::CloseImpl()
{
    std::cout << "close file\n";
	fs.close();
	return !fs.fail();
}

void* RTCFile::GetBufferImpl() {
    if (!fs.is_open()) {
        return nullptr;
    }

    // 保存当前读位置
    std::streampos originalPos = fs.tellg();
    fs.clear(); // 清理状态

    // 定位到末尾获取大小
    fs.seekg(0, std::ios::end);
    std::streampos endPos = fs.tellg();
    if (endPos < 0) {
        // 还原位置
        fs.clear();
        fs.seekg(originalPos);
        return nullptr;
    }

    size_t fileSize = static_cast<size_t>(endPos);
    fs.seekg(0, std::ios::beg);

    if (fileSize == 0) {
        // 空文件释放旧缓冲
        if (m_cachedBuffer) {
            delete[] m_cachedBuffer;
            m_cachedBuffer = nullptr;
            m_cachedBufferSize = 0;
        }
        // 还原位置
        fs.clear();
        fs.seekg(originalPos);
        return nullptr;
    }

    // 需要容量 = fileSize + 1 (终止符)
    if (fileSize + 1 > m_cachedBufferSize) {
        delete[] m_cachedBuffer;
        m_cachedBuffer = new char[fileSize + 1];
        m_cachedBufferSize = fileSize + 1;
    }

    fs.read(m_cachedBuffer, static_cast<std::streamsize>(fileSize));
    if (!fs) {
        // 读取失败
        fs.clear();
        fs.seekg(originalPos);
        return nullptr;
    }

    m_cachedBuffer[fileSize] = '\0'; // 方便当作 C 字符串使用

    // 还原原始位置（若调用方后续继续读）
    fs.clear();
    fs.seekg(originalPos);

    return m_cachedBuffer;
};

}
