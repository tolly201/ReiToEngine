#ifndef CORE_APPLICATION_RT_MAC_APPLICATION_H
#define CORE_APPLICATION_RT_MAC_APPLICATION_H

#include "Core/MinimalCore.h"
#ifdef RT_SYSTEM_APPLE
#include "RTApplication.h"

namespace ReiToEngine{
class RTMacApplication : public RTApplication
{
    friend RTApplication;
    b8 Initialize(IGame*) override;
    b8 Run() override;
    void Tick();
    void Terminate() override;
protected:
    static RTMacApplication instance;
};

}
#endif
#endif
