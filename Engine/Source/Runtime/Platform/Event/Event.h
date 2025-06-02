#ifndef PLATFORM_EVENT_EVENT_H
#define PLATFORM_EVENT_EVENT_H
#include "Core/MinimalCore.h"
#include "EventEnums.h"
#include <vector>
namespace ReiToEngine
{
struct event_context{
    union EventData {
        i64 i64[2];
        u64 u64[2];
        f64 f64[2];
        f32 f32[4];
        u32 u32[4];
        i32 i32[4];
        u16 u16[8];
        i16 i16[8];
        u8 u8[16];
        i8 i8[16];
        char* c[16];
    };
};
struct event_system_state;
using PFN_on_event = b8(*)(u32 code, void* listener, event_context context);
struct registered_event{
    void* listener;
    ReiToEngine::PFN_on_event callback;
};

struct event_code_entry{
    std::vector<registered_event> events;
};


struct event_system_state{
    // ReiToEngine::Vector<event_code_entry> registered_events;
    event_code_entry registered[U16_MAX];
};

}
#endif
