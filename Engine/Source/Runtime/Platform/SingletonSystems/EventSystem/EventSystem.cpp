#include "EventSystem.h"
#include "Platform/Application/Include/RTApplication.h"
#include "Platform/Containers/Vector.h"

namespace ReiToEngine
{

b8 EventManager::Initialize(){
    if (is_initialized) {
        RT_LOG_ERROR("EventManager is already initialized.");
        return false;
    }

    for(u32 i = 0; i < U16_MAX; ++ i)
    {
        state.registered[i].events.clear();
    }
    is_initialized = true;
    RT_LOG_INFO("EventManager initialized successfully.");
    return true;
}
b8 EventManager::Tick(){
    return true;
}
b8 EventManager::Terminate(){
    return true;
    //auto collected by container
}
b8 EventManager::RegisterEvent(u32 code, void* listener, PFN_on_event callback){
    if (!is_initialized) {
        RT_LOG_ERROR("EventManager is not initialized.");
        return false;
    }

    u64 size = state.registered[code].events.size();
    for (u64 i = 0; i < size; ++i)
    {
        if (state.registered[code].events[i].listener == listener)
        {
            //TODO need support
            return false;
        }
    }

    state.registered[code].events.emplace_back(listener, callback);
}
b8 EventManager::UnregisterEvent(u32 code, void* listener, PFN_on_event callback){
    if (!is_initialized) {
        RT_LOG_ERROR("EventManager is not initialized.");
        return false;
    }

    u64 size = state.registered[code].events.size();
    for (u64 i = 0; i < size; ++i)
    {
        if (state.registered[code].events[i].listener == listener && state.registered[code].events[i].callback == callback)
        {
            return true;
        }
    }
    return false;
}
b8 EventManager::TriggerEvent(u32 code, void* sender, event_context context){
    if (!is_initialized) {
        RT_LOG_ERROR("EventManager is not initialized.");
        return false;
    }

    u64 size = state.registered[code].events.size();
    for (u64 i = 0; i < size; ++i)
    {
        registered_event event = state.registered[code].events[i];
        if(event.callback(code, sender, context))
        {
            return true;
        }
    }
    return false;
}
}
