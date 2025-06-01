#include "Event.h"
#include "Platform/Application/Include/RTApplication.h"
#include "../Containers/Vector.h"

namespace ReiToEngine
{
struct registered_event{
    void* listener;
    ReiToEngine::PFN_on_event callback;
};

struct event_code_entry{
    ReiToEngine::Vector<registered_event> events;
};

#define MESSAGE_LENGTH sizeof(SYSTEM_EVENT_CODE)/sizeof(u8)

struct event_system_state{
    // ReiToEngine::Vector<event_code_entry> registered_events;
    event_code_entry registered[MESSAGE_LENGTH];
};

b8 EventManager::Initialize(){
    if (is_initialized) {
        RT_LOG_ERROR("EventManager is already initialized.");
        return false;
    }

    for(u32 i = 0; i < MESSAGE_LENGTH; ++ i)
    {
        state.registered[i].events.clear();
    }
    is_initialized = true;
    RT_LOG_INFO("EventManager initialized successfully.");
    return true;
}
b8 EventManager::Tick(){
}
b8 EventManager::Terminate(){
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
