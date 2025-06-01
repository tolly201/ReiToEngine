#ifndef CORE_HAL_INPUT_DEVICE_KEYBOARD_H
#define CORE_HAL_INPUT_DEVICE_KEYBOARD_H
#include "InputDevice.h"

namespace ReiToEngine {
class KeyboardDevice : public InputDevice<KeyboardDevice> {
friend class InputDevice<KeyboardDevice>;
public:
    void InnerProcessNativeEvent(void* nativeEvent)override{};
    void InnerUpdate()override{};

    b8 IsKeyDown(u16 keyCode) const;
    b8 IsKeyPressed(u16 keyCode) const;
    b8 IsKeyReleased(u16 keyCode) const;
private:
};
} // namespace ReiToEngine
#endif
