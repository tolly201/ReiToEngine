#ifndef CORE_HAL_INPUT_DEVICE_KEYBOARD_H
#define CORE_HAL_INPUT_DEVICE_KEYBOARD_H
#include "InputDevice.h"

namespace ReiToEngine {
class KeyboardDevice : public InputDevice<KeyboardDevice> {
friend class InputDevice<KeyboardDevice>;
public:
    void InnerProcessNativeEvent([[maybe_unused]]void* nativeEvent)override{};
    void InnerUpdate()override{};

    b8 IsKeyDown(KEY_CODE_KEYBOARD keyCode) const;
    b8 IsKeyUp(KEY_CODE_KEYBOARD keyCode) const;
private:
};
} // namespace ReiToEngine
#endif
