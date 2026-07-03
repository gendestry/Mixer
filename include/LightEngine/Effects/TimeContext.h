#pragma once

#include <cmath>
#include <cstdint>

//
// TimeContext: the timing handed to every effect each frame. Animation is
// expressed in real seconds so it is frame-rate independent.
//
namespace LightEngine::Effects
{
    struct TimeContext
    {
        double   elapsed = 0.0;   // seconds since the engine started
        double   dt      = 0.0;   // seconds since the previous frame
        uint32_t tick    = 0;     // frame counter

        // Sawtooth phase in [0, 1) cycling at `hz` cycles per second.
        [[nodiscard]] float phase(float hz) const
        {
            if (hz <= 0.0f) return 0.0f;
            const double p = elapsed * static_cast<double>(hz);
            return static_cast<float>(p - std::floor(p));
        }
    };
}
