#include "Effects/Effects.h"

#include <cmath>
#include <cstddef>

#include "Fixture/Fixture.h"

namespace Core::Effects
{
    namespace
    {
        Utils::Colors::RGB lerp(const Utils::Colors::RGB& a, const Utils::Colors::RGB& b, float t)
        {
            auto mix = [t](uint8_t x, uint8_t y)
            {
                return static_cast<uint8_t>(x + (static_cast<float>(y) - static_cast<float>(x)) * t);
            };
            return {mix(a.r, b.r), mix(a.g, b.g), mix(a.b, b.b)};
        }

        // Sample a curve at a normalized position in [0, 1) (wrapping).
        float sampleCurve(const Utils::Maths::Curve& c, float pos)
        {
            const uint32_t len = c.getLength();
            if (len == 0) return 0.0f;
            pos = pos - std::floor(pos);
            auto idx = static_cast<std::size_t>(pos * static_cast<float>(len));
            if (idx >= len) idx = len - 1;
            return c[idx];
        }
    }

    void ColorEffect::apply(const TimeContext&)
    {
        m_group->setColor(m_color);
    }

    void DimmerEffect::apply(const TimeContext&)
    {
        // Sets the intensity level only. Virtual-dimmer composition happens
        // once per frame in Engine::update(), after all layers have written.
        for (const auto& f : m_group->fixtures())
            f->setIntensity(m_level);
    }

    void ColorFade::apply(const TimeContext& t)
    {
        const float p = t.phase(m_hz);
        const float ping = p < 0.5f ? p * 2.0f : (1.0f - p) * 2.0f;   // 0..1..0
        m_group->setColor(lerp(m_a, m_b, ping));
    }

    DimmerChase::DimmerChase(DMX::FixtureGroup& g, Utils::Maths::Type type, float bpm, float spread, uint16_t resolution)
        : Effect(g), m_curve(Utils::Maths::getCurveByType(type, resolution)), m_bpm(bpm), m_spread(spread) {}

    void DimmerChase::apply(const TimeContext& t)
    {
        const auto& fixtures = m_group->fixtures();
        const std::size_t n = fixtures.size();
        if (n == 0) return;

        // Temporal phase advances at the BPM rate (fixture-count independent);
        // the spatial term spreads `m_spread` full waves across the group.
        const float scroll = t.phase(m_bpm / 60.0f);
        for (std::size_t i = 0; i < n; ++i)
        {
            const float pos = static_cast<float>(i) / static_cast<float>(n) * m_spread + scroll;
            const float level = sampleCurve(*m_curve, pos);
            fixtures[i]->setIntensity(level);   // vdimmer composed in Engine::update()
        }
    }
}
