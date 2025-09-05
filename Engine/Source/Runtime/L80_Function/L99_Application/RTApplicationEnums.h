#ifndef CORE_RTAPPLICATION_ENUMS_H
#define CORE_RTAPPLICATION_ENUMS_H
#include "L0_Macro/Include.h"
#include "L20_Platform/Include.h"
#include "L40_HAL/Include.h"
namespace ReiToEngine{
struct RT_MAIN_WINDOW
{
    PlatformWindow* window_ptr = nullptr;
};
struct ApplicationState
{
    ApplicationState() = default;
    ~ApplicationState() = default;
    b8 is_running;
    b8 is_paused;
    u32 width;
    u32 height;
    f64 last_time;
    i16 pos_x;
    i16 pos_y;
    Timer timer;
    RT_MAIN_WINDOW main_window;
};

struct ApplicatonConfig
{
    ApplicatonConfig() = default;
    ~ApplicatonConfig() = default;
    ApplicatonConfig(ApplicatonConfig&) = default;
    i16 start_pos_x;
    i16 start_pos_y;
    i16 start_height;
    i16 start_width;
    const char* name;
};

}
#endif
