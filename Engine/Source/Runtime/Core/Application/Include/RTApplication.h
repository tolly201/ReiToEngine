#ifndef CORE_APPLICATION_RTAPPLICATION_H
#define CORE_APPLICATION_RTAPPLICATION_H
#include "Core/Platform/Singleton/SingleonFactory.h"
namespace ReiToEngine{
class RTApplication : public SingletonFactory
{
    friend class SingletonFactory;
public:
    RTDebugHeapMemoryManager& GetMemoryManager();
    static RTApplication& Instance();
    RTApplication();
    ~RTApplication();
    void Initialize();
    void Run();
    void Tick();
protected:
    static RTApplication instance;
};
}
#endif
