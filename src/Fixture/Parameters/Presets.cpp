#include "LightEngine/Fixture/Parameters/Presets.h"

namespace LightEngine::Parameters::Presets
{
    const Descriptor* ColorRGB()
    {
        static const Descriptor d = Descriptor::make(Type::COLOR, 1, {Field::R, Field::G, Field::B});
        return &d;
    }
    const Descriptor* ColorGRB()
    {
        static const Descriptor d = Descriptor::make(Type::COLOR, 1, {Field::G, Field::R, Field::B});
        return &d;
    }
    const Descriptor* ColorBGR()
    {
        static const Descriptor d = Descriptor::make(Type::COLOR, 1, {Field::B, Field::G, Field::R});
        return &d;
    }
    const Descriptor* ColorRGBW()
    {
        static const Descriptor d = Descriptor::make(Type::COLOR, 1, {Field::R, Field::G, Field::B, Field::W});
        return &d;
    }
    const Descriptor* ColorW()
    {
        static const Descriptor d = Descriptor::make(Type::COLOR, 1, {Field::W});
        return &d;
    }
    const Descriptor* ColorRGB16()
    {
        static const Descriptor d = Descriptor::make(Type::COLOR, 2, {Field::R, Field::G, Field::B});
        return &d;
    }
    const Descriptor* Dimmer()
    {
        static const Descriptor d = Descriptor::make(Type::DIMMER, 1, {Field::DIMMER});
        return &d;
    }
    const Descriptor* Dimmer16()
    {
        static const Descriptor d = Descriptor::make(Type::DIMMER, 2, {Field::DIMMER});
        return &d;
    }
    const Descriptor* PositionPan()
    {
        static const Descriptor d = Descriptor::make(Type::POSITION, 1, {Field::PAN});
        return &d;
    }
    const Descriptor* PositionTilt()
    {
        static const Descriptor d = Descriptor::make(Type::POSITION, 1, {Field::TILT});
        return &d;
    }
    const Descriptor* Position()  // moving heads: 16-bit
    {
        static const Descriptor d = Descriptor::make(Type::POSITION, 2, {Field::PAN, Field::TILT});
        return &d;
    }
    const Descriptor* Virtual()
    {
        static const Descriptor d{Type::VDIMMER, 0, {}};
        return &d;
    }
}
