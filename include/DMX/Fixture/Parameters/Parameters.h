//
// Created by bobi on 15/09/2025.
//

#pragma once
#include "DMX/Fixture/Parameters/ColorParameter.h"
#include "DMX/Fixture/Parameters/PositionParameter.h"
#include "DMX/Fixture/Parameters/DimmerParameter.h"

#include <string_view>

#include "DMX/Fixture/Parameters/VDimmerParameter.h"

namespace DMX::Parameters
{
    using ColorRGB = ColorParameter<1, ColorType::R, ColorType::G, ColorType::B>;
    using ColorGRB = ColorParameter<1, ColorType::G, ColorType::R, ColorType::B>;
    using ColorBGR = ColorParameter<1, ColorType::B, ColorType::G, ColorType::R>;
    using ColorRGBW = ColorParameter<1, ColorType::R, ColorType::G, ColorType::B, ColorType::W>;
    using ColorW = ColorParameter<1, ColorType::W>;

    using ColorRGB16 = ColorParameter<2, ColorType::R, ColorType::G, ColorType::B>;

    using PositionPan = PositionParameter<1, PositionType::PAN>;
    using PositionTilt = PositionParameter<1, PositionType::TILT>;
    using Position = PositionParameter<1, PositionType::PAN, PositionType::TILT>;

    using VDimmer = VDimmerParameter;
    using Dimmer = DimmerParameter<1, DimmerType::DIMMER>;
}
