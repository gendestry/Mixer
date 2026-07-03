#pragma once

#include <algorithm>

#include "Parameter.h"

//
// VDimmerParameter: a virtual dimmer. Fixtures with no dedicated dimmer channel
// emulate intensity by scaling a bound color parameter. It has no physical
// channels of its own. apply() must run AFTER the color is written for the
// frame (effect -> vdimmer ordering), since it multiplies in place.
//
namespace LightEngine::Parameters
{
    class VDimmerParameter final : public Parameter
    {
        Parameter* m_color = nullptr;
        float      m_level = 1.0f;

    public:
        using Parameter::Parameter;

        void bind(Parameter* color)           { m_color = color; }
        void setLevel(float norm)             { m_level = std::clamp(norm, 0.0f, 1.0f); }
        [[nodiscard]] float getLevel() const  { return m_level; }

        [[nodiscard]] std::shared_ptr<Parameter> clone() const override;

        void apply();
    };
}
