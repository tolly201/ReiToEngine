#ifndef CORE_HAL_SINGLETON_H
#define CORE_HAL_SINGLETON_H
#include "Core/HAL/Macro/Macro.h"
namespace ReiToEngine
{
template <typename T>
class RTENGINE_API RTSingleton
{
public:
    RT_FORCEINLINE static T& Instance() { return _instance; }
    virtual ~RTSingleton() = 0;
protected:
    static T _instance;
};

template <typename T>
RTSingleton<T>::~RTSingleton() = default;

template <typename T>
T RTSingleton<T>::_instance;
}

#endif
