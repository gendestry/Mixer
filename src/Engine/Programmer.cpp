#include "Engine/Programmer.h"

#include <algorithm>

#include "Fixture/Fixture.h"

namespace Core::Engine
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

    void Programmer::setColor(const Utils::Colors::RGB& color)
    {
        for (const auto& f : m_selection)
            m_values[f].color = color;
        touch();
    }

    void Programmer::setIntensity(float level)
    {
        for (const auto& f : m_selection)
            m_values[f].intensity = level;
        touch();
    }

    DMX::FixtureGroup& Programmer::snapshotSelection()
    {
        m_effectGroups.emplace_back("programmer");
        DMX::FixtureGroup& g = m_effectGroups.back();
        for (const auto& f : m_selection)
            g.add(f);
        return g;
    }

    Effects::DimmerChase* Programmer::addDimmerChase(float bpm, Utils::Maths::Type type)
    {
        return addEffect<Effects::DimmerChase>(type, bpm);
    }

    void Programmer::apply(const Effects::TimeContext& t)
    {
        const Utils::Colors::RGB white{255, 255, 255};

        // 1. Default colour = white for every fixture the programmer drives, so
        //    intensity with no programmed colour still produces (white) light.
        for (auto& [fix, st] : m_values)
            fix->setColor(white);
        for (auto& g : m_effectGroups)
            for (const auto& f : g.fixtures())
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
        m_effectGroups.clear();
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
