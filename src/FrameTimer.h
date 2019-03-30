#pragma once

#include <ctime>
#include <sys/utime.h>
#include <chrono>

namespace sec
{
    class FrameTimer
    {
    public:
        FrameTimer();
        void update();
        float FrameTimer::getFrameDuration();
    private:
        __int64 lastFrameTime;
        float delta; // time to render previous frame
    };
}
