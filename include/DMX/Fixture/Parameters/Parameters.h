//
// Created by bobi on 15/09/2025.
//

#pragma once
#include "DMX/Fixture/Parameters/ColorParameter.h"
#include "DMX/Fixture/Parameters/PositionParameter.h"
#include "DMX/Fixture/Parameters/DimmerParameter.h"

namespace DMX::Parameters
{
    using ColorRGB = ColorParameter<1, ColorType::R, ColorType::G, ColorType::B>;
    using ColorGRB = ColorParameter<2, ColorType::G, ColorType::R, ColorType::B>;
    using ColorRGB16 = ColorParameter<2, ColorType::R, ColorType::G, ColorType::B>;
    using ColorRGBW = ColorParameter<1, ColorType::R, ColorType::G, ColorType::B, ColorType::W>;
    using ColorW = ColorParameter<1, ColorType::W>;

    using PositionPan = PositionParameter<1, PositionType::PAN>;
    using PositionTilt = PositionParameter<1, PositionType::TILT>;

    using Dimmer = DimmerParameter<1, DimmerType::DIMMER>;
}
