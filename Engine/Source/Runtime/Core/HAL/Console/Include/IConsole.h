#ifndef CORE_HAL_CONLOLE_ICONSOLE_H
#define CORE_HAL_CONLOLE_ICONSOLE_H

#include "../../../InnerCore.h"

class RTENGINE_API IConsole
{
public:
    virtual ~IConsole() = 0;
    virtual void Write(const char* message, u8 color) = 0;
    virtual void WriteError(const char* message, u8 color) = 0;
    virtual void Update() = 0; // 更新显示 buffer
    virtual void Initialize() = 0; // 更新显示 buffer
    virtual void Terminate() = 0; // 更新显示 buffer
protected:
};
RT_FORCEINLINE IConsole::~IConsole() = default;
#endif
