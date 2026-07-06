#include "LightEngine/Engine/Programmer.h"

#include <algorithm>
#include <cstdint>
#include <utility>

#include "LightEngine/Effects/EffectFactory.h"
#include "LightEngine/Fixture/Fixture.h"

namespace
{
    // Linear interpolation between two colours (per 8-bit channel).
    Utils::Colors::RGB lerpRGB(const Utils::Colors::RGB& a, const Utils::Colors::RGB& b, float t)
    {
        const auto mix = [t](uint8_t x, uint8_t y) {
            const float v = static_cast<float>(x) + (static_cast<float>(y) - static_cast<float>(x)) * t;
            return static_cast<uint8_t>(v);
        };
        return Utils::Colors::RGB{mix(a.r, b.r), mix(a.g, b.g), mix(a.b, b.b)};
    }
}

namespace LightEngine::Engine
{
    void Programmer::select(DMX::FixtureGroup& group)
    {
        if (m_latched)       // first select after an edit -> start a fresh selection
        {
            m_selection.clear();
            m_selectedGroups.clear();
            m_latched = false;
        }
        addToSelection(group);
    }

    void Programmer::addToSelection(DMX::FixtureGroup& group)
    {
        for (const auto& f : group.fixtures())
            if (std::find(m_selection.begin(), m_selection.end(), f) == m_selection.end())
                m_selection.push_back(f);
    }

    void Programmer::clearSelection()
    {
        m_selection.clear();
        m_selectedGroups.clear();
        m_latched = false;
    }

    std::vector<uint16_t> Programmer::selectionFids() const
    {
        std::vector<uint16_t> fids;
        fids.reserve(m_selection.size());
        for (const auto& f : m_selection)
            fids.push_back(f->fid());
        return fids;
    }

    DMX::FixtureGroup Programmer::selectionGroup() const
    {
        DMX::FixtureGroup g("programmer");
        for (const auto& f : m_selection)
            g.add(f);
        return g;
    }

    void Programmer::setColor(const Utils::Colors::RGB& color)
    {
        for (const auto& f : m_selection)
        {
            m_values[f].color = color;
            m_driven.insert(f);
        }
        touch();
    }

    void Programmer::setIntensity(float level)
    {
        for (const auto& f : m_selection)
        {
            m_values[f].intensity = level;
            m_driven.insert(f);
        }
        touch();
    }

    Effects::Effect* Programmer::addEffect(Effects::Spec spec)
    {
        spec.fids = selectionFids();
        for (const auto& f : m_selection)
            m_driven.insert(f);

        auto live = Effects::EffectFactory::build(spec, selectionGroup());
        Effects::Effect* raw = m_effects.add(std::move(live));
        touch();
        return raw;
    }

    Effects::DimmerChase* Programmer::addDimmerChase(float bpm, Utils::Maths::Type type)
    {
        Effects::Spec spec;
        spec.kind  = Effects::Kind::DimmerChase;
        spec.curve = type;
        spec.bpm   = bpm;
        return static_cast<Effects::DimmerChase*>(addEffect(spec));
    }

    void Programmer::applyColorGradient(const Utils::Colors::RGB& a, const Utils::Colors::RGB& b)
    {
        const std::size_t n = m_selection.size();
        for (std::size_t i = 0; i < n; ++i)
        {
            const float t = (n <= 1) ? 0.0f : static_cast<float>(i) / static_cast<float>(n - 1);
            const auto& f = m_selection[i];
            m_values[f].color = lerpRGB(a, b, t);
            m_driven.insert(f);
        }
        touch();
    }

    void Programmer::applyPreset(const Attributes::Preset& preset)
    {
        const Attributes::Feature feature = preset.feature();

        if (preset.mode() == Attributes::RecallMode::Absolute)
        {
            // Each selected fixture gets the value stored against its own FID.
            for (const auto& f : m_selection)
            {
                if (const LightEngine::Values* v = preset.valuesFor(f->fid()))
                {
                    Attributes::mergeFeature(m_values[f], *v, feature);
                    m_driven.insert(f);
                }
            }
        }
        else   // ByIndex: fan the stored values across the ordered selection
        {
            const std::vector<LightEngine::Values> vals = preset.ordered();
            if (!vals.empty())
            {
                for (std::size_t i = 0; i < m_selection.size(); ++i)
                {
                    const auto& f = m_selection[i];
                    Attributes::mergeFeature(m_values[f], vals[i % vals.size()], feature);
                    m_driven.insert(f);
                }
            }
        }
        touch();
    }

    std::map<uint16_t, LightEngine::Values> Programmer::snapshot(Attributes::Feature feature) const
    {
        std::map<uint16_t, LightEngine::Values> out;
        for (const auto& [f, values] : m_values)
        {
            const LightEngine::Values masked = Attributes::maskFeature(values, feature);
            if (Attributes::hasFeature(masked, feature))
                out[f->fid()] = masked;
        }
        return out;
    }

    std::vector<Effects::Spec> Programmer::effectSpecs() const
    {
        std::vector<Effects::Spec> specs;
        specs.reserve(m_effects.size());
        for (const auto& fx : m_effects.effects())
            specs.push_back(fx->spec());
        return specs;
    }

    Show::Cue Programmer::makeCue(float number) const
    {
        Show::Cue cue;
        cue.number = number;
        for (const auto& [fix, values] : m_values)
            cue.values[fix->fid()] = values;
        for (const auto& fx : m_effects.effects())
            cue.effects.push_back(fx->spec());
        return cue;
    }

    void Programmer::apply(const Effects::TimeContext& t)
    {
        const Utils::Colors::RGB white{255, 255, 255};

        // 1. Default colour = white for every fixture the programmer drives.
        for (const auto& f : m_driven)
            f->setColor(white);

        // 2. Base values (override the white default where a colour was set).
        for (auto& [fix, st] : m_values)
        {
            if (st.color)     fix->setColor(*st.color);
            if (st.intensity) fix->setIntensity(*st.intensity);
        }

        // 3. Effects on top (LTP). Virtual-dimmer composition is done once per
        //    frame by the Engine after all layers have written.
        m_effects.update(t);
    }

    void Programmer::clear()
    {
        m_selection.clear();
        m_selectedGroups.clear();
        m_values.clear();
        m_driven.clear();
        m_effects.clear();
        m_latched = false;
    }

    bool Programmer::empty() const
    {
        return m_values.empty() && m_effects.size() == 0;
    }

    std::string Programmer::describe() const
    {
        return "Programmer: " + std::to_string(m_selection.size()) + " selected, "
             + std::to_string(m_values.size()) + " fixtures with values, "
             + std::to_string(m_effects.size()) + " effects";
    }
}
