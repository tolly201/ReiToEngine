#ifndef CORE_HAL_SYSTEM_SYSTEMINFO_H
#define CORE_HAL_SYSTEM_SYSTEMINFO_H
#include "Core/Macro/Macro.h"
#include "Platform/Singleton/SingletonFactory.h"
#include <cstdint>

namespace ReiToEngine
{
class RTENGINE_API SystemInfo : public Singleton<SystemInfo>
{
friend class Singleton<SystemInfo>;
public:
    RT_FORCEINLINE uint32_t GetSystemBitWidth() const
    {
        return m_SystemBitWidth;
    }
    RT_FORCEINLINE bool Is64BitSystem() const
    {
        return m_SystemBitWidth == 64;
    }
    RT_FORCEINLINE bool Is32BitSystem() const
    {
        return m_SystemBitWidth == 32;
    }
protected:
    SystemInfo() = default;
    ~SystemInfo() = default;
private:
    const uint32_t m_SystemBitWidth = RT_SYSTEM_BIT_WIDTH;
};
}
#endif
