#include "LightEngine/DMX/FixtureGroup.h"

#include <algorithm>
#include <utility>

namespace LightEngine::DMX
{
    FixtureGroup::FixtureGroup(std::string name) : m_name(std::move(name)) {}

    void FixtureGroup::rebuildCache() const
    {
        m_parameters.clear();
        for (const auto& f : m_fixtures)
            for (const auto& p : f->parameters())
                m_parameters[p->getType()].push_back(p);
        m_cacheDirty = false;
    }

    bool FixtureGroup::contains(const FixturePtr& fixture) const
    {
        return std::find(m_fixtures.begin(), m_fixtures.end(), fixture) != m_fixtures.end();
    }

    void FixtureGroup::add(const FixturePtr& fixture)
    {
        if (fixture == nullptr || contains(fixture)) return;
        m_fixtures.push_back(fixture);
        m_usedUniverses.insert(fixture->universe());
        m_cacheDirty = true;
    }

    void FixtureGroup::add(const std::vector<FixturePtr>& fixtures)
    {
        for (const auto& f : fixtures) add(f);
    }

    void FixtureGroup::add(const FixtureGroup& other)
    {
        add(other.m_fixtures);
    }

    void FixtureGroup::clear()
    {
        m_fixtures.clear();
        m_usedUniverses.clear();
        m_parameters.clear();
        m_cacheDirty = true;
    }

    const std::vector<FixtureGroup::ParamPtr>& FixtureGroup::parameters(Parameters::Type t) const
    {
        static const std::vector<ParamPtr> empty;
        if (m_cacheDirty) rebuildCache();
        const auto it = m_parameters.find(t);
        return it != m_parameters.end() ? it->second : empty;
    }

    bool FixtureGroup::has(Parameters::Type t) const
    {
        if (m_cacheDirty) rebuildCache();
        return m_parameters.contains(t);
    }

    void FixtureGroup::setColor(const Utils::Colors::RGB& color)
    {
        for (const auto& f : m_fixtures) f->setColor(color);
    }

    void FixtureGroup::setIntensity(float norm)
    {
        for (const auto& f : m_fixtures) f->setIntensity(norm);
    }

    void FixtureGroup::applyVirtualDimmers()
    {
        for (const auto& f : m_fixtures) f->applyVirtualDimmers();
    }

    FixtureGroup& FixtureGroup::operator+=(const FixturePtr& fixture)
    {
        add(fixture);
        return *this;
    }

    FixtureGroup& FixtureGroup::operator+=(const FixtureGroup& other)
    {
        add(other);
        return *this;
    }

    std::string FixtureGroup::describe() const
    {
        std::string s = "Group \"" + m_name + "\" [" + std::to_string(m_fixtures.size())
                      + " fixtures, " + std::to_string(m_usedUniverses.size()) + " universes]";
        for (const auto& f : m_fixtures)
            s += "\n  - " + f->describe();
        return s;
    }
}
