#ifndef PLATFORM_HAL_INPUT_DEVICE_MOUSE_H
#define PLATFORM_HAL_INPUT_DEVICE_MOUSE_H

#include "InputDevice.h"
namespace ReiToEngine {
// 鼠标设备
class MouseDevice : public InputDevice<MouseDevice> {
friend class InputDevice<MouseDevice>;
public:
    void InnerProcessNativeEvent([[maybe_unused]]void* nativeEvent)override{};
    void InnerUpdate()override{};

    bool IsButtonDown(int button) const;
    float GetScrollDelta() const;
private:
    float m_scrollDelta;
};

}
#endif
