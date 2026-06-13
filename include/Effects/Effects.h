#pragma once

#include <cstdint>
#include <memory>

#include "Utils/Colors/RGB.h"
#include "Utils/Math/Curve.h"

#include "Effects/Effect.h"

//
// Concrete effects. Static ones ignore time; dynamic ones animate off the
// TimeContext. Per-fixture effects use the fixture's index within the group.
//
namespace Core::Effects
{
    // Holds the group at a constant colour.
    class ColorEffect : public Effect
    {
        Utils::Colors::RGB m_color;

    public:
        ColorEffect(DMX::FixtureGroup& g, Utils::Colors::RGB color) : Effect(g), m_color(color) {}
        void setColor(Utils::Colors::RGB color) { m_color = color; }
        void apply(const TimeContext& t) override;
    };

    // Holds the group at a constant intensity (real dimmer or virtual).
    class DimmerEffect : public Effect
    {
        float m_level;

    public:
        DimmerEffect(DMX::FixtureGroup& g, float level) : Effect(g), m_level(level) {}
        void setLevel(float level) { m_level = level; }
        void apply(const TimeContext& t) override;
    };

    // Ping-pongs the group colour between two colours at `hz`.
    class ColorFade : public Effect
    {
        Utils::Colors::RGB m_a;
        Utils::Colors::RGB m_b;
        float              m_hz;

    public:
        ColorFade(DMX::FixtureGroup& g, Utils::Colors::RGB a, Utils::Colors::RGB b, float hz = 0.5f)
            : Effect(g), m_a(a), m_b(b), m_hz(hz) {}
        void apply(const TimeContext& t) override;
    };

    // A travelling intensity wave across the group, shaped by a curve.
    //   bpm    - temporal rate: cycles per minute, independent of fixture count.
    //   spread - spatial distribution: number of full waves across the group
    //            (1 = one wave end-to-end, 0 = all fixtures in sync, 2 = two waves).
    class DimmerChase : public Effect
    {
        std::unique_ptr<Utils::Maths::Curve> m_curve;
        float                                m_bpm;
        float                                m_spread;

    public:
        DimmerChase(DMX::FixtureGroup& g, Utils::Maths::Type type = Utils::Maths::SINUSOID,
                    float bpm = 60.0f, float spread = 1.0f, uint16_t resolution = 128);

        void setBpm(float bpm)          { m_bpm = bpm; }       // runtime tempo control
        [[nodiscard]] float bpm() const { return m_bpm; }
        void setSpread(float spread)    { m_spread = spread; }
        [[nodiscard]] float spread() const { return m_spread; }

        void apply(const TimeContext& t) override;
    };
}
