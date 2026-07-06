#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "Utils/Colors/RGB.h"

#include "LightEngine/Fixture/Fixture.h"
#include "LightEngine/Fixture/Parameters/Channel.h"
#include "LightEngine/Fixture/Parameters/Parameter.h"

//
// FixtureGroup: a named selection of fixtures driven together. It does not own
// the fixtures (the universes do) - it holds shared references to them and a
// cache of their parameters bucketed by Type, so an effect can grab "every
// color parameter in this group" in one shot. It also tracks which universes
// the group spans, for dirty-marking.
//
namespace LightEngine::DMX
{
    class FixtureGroup
    {
        using FixturePtr = std::shared_ptr<LightEngine::Fixture>;
        using ParamPtr   = std::shared_ptr<Parameters::Parameter>;

        std::string                                 m_name;
        std::vector<FixturePtr>                     m_fixtures;
        std::set<uint16_t>                          m_usedUniverses;
        mutable std::map<Parameters::Type, std::vector<ParamPtr>> m_parameters;  // cached, by type (lazy)
        mutable bool                                m_cacheDirty = true;

        void rebuildCache() const;   // (re)build m_parameters from m_fixtures

    public:
        FixtureGroup() = default;
        explicit FixtureGroup(std::string name);

        // Membership (ignores nulls and duplicates).
        void add(const FixturePtr& fixture);
        void add(const std::vector<FixturePtr>& fixtures);
        void add(const FixtureGroup& other);   // merge another group's fixtures
        void clear();

        [[nodiscard]] const std::string& name() const { return m_name; }
        void setName(std::string name) { m_name = std::move(name); }

        [[nodiscard]] const std::vector<FixturePtr>& fixtures() const { return m_fixtures; }
        [[nodiscard]] std::size_t size() const { return m_fixtures.size(); }
        [[nodiscard]] bool empty() const { return m_fixtures.empty(); }
        [[nodiscard]] bool contains(const FixturePtr& fixture) const;
        [[nodiscard]] const std::set<uint16_t>& usedUniverses() const { return m_usedUniverses; }

        // Every parameter of a given type across all member fixtures (cached).
        [[nodiscard]] const std::vector<ParamPtr>& parameters(Parameters::Type t) const;
        [[nodiscard]] bool has(Parameters::Type t) const;

        // ---- convenience: apply to the whole group ----
        void setColor(const Utils::Colors::RGB& color);
        void setIntensity(float norm);
        void applyVirtualDimmers();

        FixtureGroup& operator+=(const FixturePtr& fixture);
        FixtureGroup& operator+=(const FixtureGroup& other);

        [[nodiscard]] std::string describe() const;
    };
}
