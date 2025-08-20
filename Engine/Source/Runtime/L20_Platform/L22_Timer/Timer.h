#ifndef PLATFORM_TIMER_H
#define PLATFORM_TIMER_H
#include "L0_Macro/Include.h"
#endif


namespace ReiToEngine{

class Timer
{
public:
    Timer();
    ~Timer();
    b8 Initialize();
    b8 Terminate();
    b8 Tick();
    const f64& deltaTime;
    const f64& time;
    const f64& realtimeSinceStartup;
    const f64& timeScale;
    const f64& targetFrameTime;
    const u32& frameCount;
    void SetFrameCount(u32 count);
    private:
    f64 time_scale = 1.0;
    f64 target_frame_time = 0;
    u32 frame_count = 0;
    f64 start_time = 0;
    f64 delta_time = 0;
    f64 elasped_time = 0;
    f64 now_time = 0;
};
}
