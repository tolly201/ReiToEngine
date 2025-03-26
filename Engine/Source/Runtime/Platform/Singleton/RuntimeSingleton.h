#ifndef CORE_HAL_RUNTIME_SINGLETON_H
#define CORE_HAL_RUNTIME_SINGLETON_H
#include "SingletonFactory.h"
namespace ReiToEngine {
template <typename T>
class RTENGINE_API Runtime_Singleton : public Singleton<T>
{
    virtual void Initialize() = 0;
    virtual void Tick() = 0;
    virtual void Terminate() = 0;
protected:
    Runtime_Singleton() = default;
    virtual ~Runtime_Singleton() = default;
};

}  // namespace ReiToEngine
#endif
