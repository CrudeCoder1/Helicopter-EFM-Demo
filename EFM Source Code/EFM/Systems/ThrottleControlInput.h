#pragma once

#include <algorithm>

// Throttle mapping from the working UH_EFM carrier, kept independent of its
// engine-failure simulation and aircraft-specific device command numbers.
namespace ThrottleControlInput
{
    struct AxisCommand
    {
        double cockpitPosition = 0.0;
        double engineCommand = 0.0;
    };

    struct IdleCutoffCommand
    {
        double engineCommand = 0.0;
        bool engaged = false;
    };

    inline AxisCommand commandFromDcsAxis(double value)
    {
        // DCS: -1 full open, +1 idle. Capture the final two percent of
        // travel so ordinary hardware calibration reaches the flight detent.
        AxisCommand result;
        result.cockpitPosition = std::max(0.0, std::min(1.0, (1.0 - value) / 2.0));
        if (result.cockpitPosition >= 0.98)
            result.cockpitPosition = 1.0;
        result.engineCommand = result.cockpitPosition;
        return result;
    }

    inline IdleCutoffCommand commandFromIdleCutoff(double value, double currentThrottle)
    {
        IdleCutoffCommand result;
        result.engaged = value > 0.5;
        result.engineCommand =
            result.engaged ? -1.0 : std::max(0.0, std::min(1.0, currentThrottle));
        return result;
    }
} // namespace ThrottleControlInput
