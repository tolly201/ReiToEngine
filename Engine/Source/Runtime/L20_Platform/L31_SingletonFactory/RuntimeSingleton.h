#ifndef PLATFORM_RUNTIME_SINGLETON_H
#define PLATFORM_RUNTIME_SINGLETON_H
#include "SingletonFactory.h"
namespace ReiToEngine {
template <typename T>
class RTENGINE_API Runtime_Singleton : public Singleton<T>
{
    virtual b8 Initialize() = 0;
    virtual b8 Tick(f64) = 0;
    virtual b8 Terminate() = 0;
protected:
    Runtime_Singleton() = default;
    virtual ~Runtime_Singleton() = default;
};

}  // namespace ReiToEngine
#endif
