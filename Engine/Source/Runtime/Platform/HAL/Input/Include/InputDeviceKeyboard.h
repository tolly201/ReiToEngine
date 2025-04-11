#ifndef PLATFORM_HAL_INPUT_DEVICE_KEYBOARD_H
#define PLATFORM_HAL_INPUT_DEVICE_KEYBOARD_H
#include "InputDevice.h"

namespace ReiToEngine {
// 键盘设备
class KeyboardDevice : public InputDevice {
public:
    void Update() override;
    void ProcessNativeEvent(void* nativeEvent) override;
    bool IsConnected() const override { return true; }

    // 键盘特有方法
    bool IsKeyDown(int keyCode) const;
    bool IsKeyPressed(int keyCode) const;
    bool IsKeyReleased(int keyCode) const;

private:
    std::array<bool, 256> m_currentKeyStates;
    std::array<bool, 256> m_previousKeyStates;
};
} // namespace ReiToEngine
#endif
