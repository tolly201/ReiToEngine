// Engine/Source/Runtime/Platform/HAL/Input/Include/IInputSystem.h
#ifndef CORE_PLATFORM_INPUTSYSTEM_INPUTSUSTEM_H
#define CORE_PLATFORM_INPUTSYSTEM_INPUTSUSTEM_H
#include <functional>
#include <memory>
#include "Core/Macro/Macro.h"
#include "Platform/HAL/Input/Input.h"
#include "Platform/Singleton/RuntimeSingleton.h"

namespace ReiToEngine {

// 输入系统接口
class InputSystem : public Runtime_Singleton<InputSystem> {
   public:
    InputSystem();
    ~InputSystem();

    // 注册输入事件回调
    void AddInputCallback(EINPUT_EVENT_TYPE type, InputCallback callback);

    // 移除特定类型的所有回调
    void RemoveInputCallbacks(EINPUT_EVENT_TYPE type);

    // 处理输入事件
    void ProcessInputEvent(const InputEvent& event);

        // 更新输入状态
    void Initialize();
    void Terminate();
    void Tick();
   private:
    std::unordered_map<EINPUT_DEVICE_TYPE, std::vector<InputDevice*>> m_devices;
    std::vector<InputEvent> eventList;

    std::unordered_map<EINPUT_EVENT_TYPE, std::vector<InputCallback>> callbacks;
    InputMonitor* m_eventMonitor;
};

}  // namespace ReiToEngine
#endif
