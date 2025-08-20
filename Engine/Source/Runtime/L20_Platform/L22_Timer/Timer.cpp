#include "Timer.h"
#include "Functions.h"
namespace ReiToEngine{

Timer::Timer() : deltaTime(delta_time), time(now_time), realtimeSinceStartup(elasped_time), timeScale(time_scale), targetFrameTime(target_frame_time), frameCount(frame_count) {
    delta_time = 0.0;
    elasped_time = 0.0;
    start_time = 0.0;
    now_time = 0.0;
    frame_count = 0;
    target_frame_time = 0.0;
}
Timer::~Timer() = default;


void Timer::SetFrameCount(u32 count)
{
    if (count > 0)
    {
        frame_count = count;
        target_frame_time = 1.0 / static_cast<f64>(frame_count);
    }
}

b8 Timer::Initialize() {
    RT_Platform_InitTime();
    start_time = RT_Platform_AbsoluteTime();
    elasped_time = 0.0;
    delta_time = 0.0;
    frame_count = 60.0;
    target_frame_time = 1.0 / frame_count;
    now_time = start_time;
    return true;
}

b8 Timer::Terminate() {
    start_time = 0.0;
    elasped_time = 0.0;
    delta_time = 0.0;
    return true;
}

b8 Timer::Tick() {
    f64 current_time = RT_Platform_AbsoluteTime();
    delta_time = (current_time - now_time);
    elasped_time = current_time - start_time;
    now_time = current_time;
    return true;
}

}
