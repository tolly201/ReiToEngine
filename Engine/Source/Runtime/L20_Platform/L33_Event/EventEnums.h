#ifndef PLATFORM_EVENT_EVENT_ENUMS_H
#define PLATFORM_EVENT_EVENT_ENUMS_H
#include "L0_Macro/Include.h"
#include "L20_Platform/L32_Containers/Vector.h"
namespace ReiToEngine {
enum SYSTEM_EVENT_CODE : u8 {
    UNKNOWN = 0x00,
    APPLICATION_QUIT = 0x01,

    KEY_PRESS = 0x02,
    KEY_RELEASE = 0x03,
    KEY_REPEAT = 0x04,

    MOUSE_BUTTON_PRESS = 0x05,
    MOUSE_BUTTON_RELEASE = 0x06,
    MOUSE_BUTTON_REPEAT = 0x07,

    MOUSE_MOVE = 0x08,
    MOUSE_WHEEL = 0x09,

    WINDOW_RESIZE = 0x0A,

    DEVICE_CONNECTED = 0x0B,
    DEVICE_DISCONNECTED = 0x0C,

    MAX_EVENT_CODE = 0xFF
};


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
    } data;
};
struct event_system_state;
using PFN_on_event = b8(*)(u16 code, void* sender, void* listener, event_context context);
struct registered_event{
    void* listener;
    ReiToEngine::PFN_on_event callback;

    registered_event(void* l, PFN_on_event cb)
        : listener(l), callback(cb) {}
};

struct event_code_entry{
    ReiToEngine::vector<registered_event> events;
    ~event_code_entry() = default;
};

struct event_system_state{
    event_code_entry registered[U16_MAX];
    ~event_system_state() = default;
};
}
#endif
