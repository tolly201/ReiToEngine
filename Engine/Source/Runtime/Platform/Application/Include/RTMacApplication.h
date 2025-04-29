#ifndef CORE_APPLICATION_RT_MAC_APPLICATION_H
#define CORE_APPLICATION_RT_MAC_APPLICATION_H
#include "RTApplication.h"

namespace ReiToEngine{
class RTMacApplication : public RTApplication
{
    friend RTApplication;
    void Initialize(ApplicatonConfig&) override;
    void Run() override;
    void Tick() override;
    void Terminate() override;
protected:
    static RTMacApplication instance;
};

}
#endif
