#ifndef PLATFORM_HAL_INPUT_DEVICE_MOUSE_H
#define PLATFORM_HAL_INPUT_DEVICE_MOUSE_H

#include "InputDevice.h"
namespace ReiToEngine {
// 鼠标设备
class MouseDevice : public InputDevice {
public:
    void Update() override;
    void ProcessNativeEvent(void* nativeEvent) override;
    bool IsConnected() const override { return true; }

    // // 鼠标特有方法
    // glm::vec2 GetPosition() const;
    // glm::vec2 GetDelta() const;
    bool IsButtonDown(int button) const;
    float GetScrollDelta() const;

private:
    // glm::vec2 m_currentPosition;
    // glm::vec2 m_previousPosition;
    // glm::vec2 m_delta;
    float m_scrollDelta;
    // std::array<bool, 5> m_buttonStates;
};

} // namespace ReiToEngine
#endif
