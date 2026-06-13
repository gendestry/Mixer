#pragma once

#include "Descriptor.h"

//
// Presets: descriptors for common hardcoded personalities and tests. Each
// returns a shared, immortal Descriptor (function-local static), so pointers to
// it are always valid. File-loaded fixtures use a DescriptorStore instead; both
// produce the same Descriptor type.
//
namespace Core::Parameters::Presets
{
    const Descriptor* ColorRGB();
    const Descriptor* ColorGRB();
    const Descriptor* ColorBGR();
    const Descriptor* ColorRGBW();
    const Descriptor* ColorW();
    const Descriptor* ColorRGB16();

    const Descriptor* Dimmer();
    const Descriptor* Dimmer16();

    const Descriptor* PositionPan();
    const Descriptor* PositionTilt();
    const Descriptor* Position();   // moving heads: 16-bit

    const Descriptor* Virtual();
}
