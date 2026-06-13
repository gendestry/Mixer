#include "Engine/Components/FixtureLibrary.h"

namespace Core::Engine::Components
{
    Core::Fixture& FixtureLibrary::define(const std::string& name)
    {
        auto it = m_fixtures.find(name);
        if (it == m_fixtures.end())
            it = m_fixtures.emplace(name, Core::Fixture(name)).first;
        return it->second;
    }

    void FixtureLibrary::add(const Core::Fixture& fixture)
    {
        add(fixture.name(), fixture);
    }

    void FixtureLibrary::add(const std::string& name, const Core::Fixture& fixture)
    {
        m_fixtures.insert_or_assign(name, fixture);
    }

    const Core::Fixture* FixtureLibrary::get(const std::string& name) const
    {
        const auto it = m_fixtures.find(name);
        return it != m_fixtures.end() ? &it->second : nullptr;
    }

    bool FixtureLibrary::has(const std::string& name) const
    {
        return m_fixtures.contains(name);
    }

    std::string FixtureLibrary::describe() const
    {
        std::string s = "FixtureLibrary [" + std::to_string(m_fixtures.size()) + " definitions]";
        for (const auto& [name, fix] : m_fixtures)
            s += "\n  - " + name + " (" + std::to_string(fix.footprint()) + " ch)";
        return s;
    }
}
