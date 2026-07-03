#include "LightEngine/Engine/Programmer.h"

#include <algorithm>
#include <utility>

#include "LightEngine/Effects/EffectFactory.h"
#include "LightEngine/Fixture/Fixture.h"

namespace LightEngine::Engine
{
    void Programmer::select(DMX::FixtureGroup& group)
    {
        if (m_latched)       // first select after an edit -> start a fresh selection
        {
            m_selection.clear();
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
