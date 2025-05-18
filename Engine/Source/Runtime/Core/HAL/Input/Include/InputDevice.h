#ifndef PLATFORM_HAL_INPUT_DEVICE_H
#define PLATFORM_HAL_INPUT_DEVICE_H
namespace ReiToEngine {

class InputDevice {
public:
    virtual ~InputDevice() = default;
    virtual void Update() = 0;
    virtual void ProcessNativeEvent(void* nativeEvent) = 0;
    virtual bool IsConnected() const = 0;
};
} // namespace ReiToEngine
#endif
