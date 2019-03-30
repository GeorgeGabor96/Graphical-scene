#include "FrameTimer.h"

namespace sec
{
    FrameTimer::FrameTimer() 
    {
        lastFrameTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        delta = 0.0f;
    };

    void FrameTimer::update()
    {
        time_t currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        delta = (float)(currentTime - lastFrameTime) / 1000;
        lastFrameTime = currentTime;
    }

    float FrameTimer::getFrameDuration()
    {
        return delta;
    }
}