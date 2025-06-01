#ifndef CORE_HAL_INPUT_DEVICE_H
#define CORE_HAL_INPUT_DEVICE_H
#include "InputEnums.h"
namespace ReiToEngine {
template <typename T>
class InputDevice {
public:
    virtual ~InputDevice();
    void Update(){
        static_cast<T*>(this)->InnerUpdate();
    }
    void ProcessNativeEvent(void* nativeEvent)
    {
        static_cast<T*>(this)->InnerProcessNativeEvent(nativeEvent);
    }
    bool IsConnected() const {
        return isConnected;
    };
protected:
    virtual void InnerProcessNativeEvent(void* nativeEvent) = 0;
    virtual void InnerUpdate() = 0;
    b8 isConnected = false;
    INPUT_DEVICE_TYPE deviceType = INPUT_DEVICE_TYPE::UNKNOWN;
};
} // namespace ReiToEngine
#endif
