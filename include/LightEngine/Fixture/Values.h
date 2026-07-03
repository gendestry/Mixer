#pragma once

#include <optional>

#include "Utils/Colors/RGB.h"

//
// Values: the programmable attribute values for one fixture. An unset (nullopt)
// attribute is simply not driven. Shared by the Programmer (live edits) and the
// Cue (stored look).
//
namespace LightEngine
{
    struct Values
    {
        std::optional<Utils::Colors::RGB> color;
        std::optional<float>              intensity;
    };
}
