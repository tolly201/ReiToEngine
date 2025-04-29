#ifndef CORE_APPLICATION_RTAPPLICATION_ENUMS_H
#define CORE_APPLICATION_RTAPPLICATION_ENUMS_H
#include "Core/MinimalCore.h"

namespace ReiToEngine{
struct ApplicatonState
{
    ApplicatonState() = default;
    ~ApplicatonState() = default;
    bool is_running;
    bool is_paused;
    i16 width;
    i16 height;
    f64 last_time;
    i16 pos_x;
    i16 pos_y;
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
    char* name;
};

}
#endif
