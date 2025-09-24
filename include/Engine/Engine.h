//
// Created by bobi on 24/09/2025.
//

#pragma once
#include <cstdint>
#include <set>
#include <map>

#include "DMX/FixtureGroup.h"
#include "DMX/Universe.h"

class Engine
{
    // std::unordered_map<std::string, DMX::Fixture> m_fixtures;
    std::unordered_map<uint8_t, DMX::Universe> m_universes;
    std::vector<DMX::FixtureGroup> m_groups;
    std::map<uint16_t, std::shared_ptr<DMX::Fixture>> m_fixtures;
    std::set<uint16_t> m_usedFids;

public:

    void patch(DMX::Fixture& fixture, uint8_t universe, uint16_t amount, std::optional<uint16_t> start = std::nullopt, std::optional<uint16_t> startFID = std::nullopt)
    {
        // static uint16_t fidCounter = 0U;
        uint16_t fid = startFID.has_value() ? startFID.value() : universe * 100 + 1;
        bool canUseFid = true;
        for (uint16_t i = 0; i < amount; ++i)
        {
            if (m_usedFids.contains(fid + i))
            {
                canUseFid = false;
                break;
            }
        }

        if (!canUseFid)
        {
            throw std::runtime_error("FIDS are in use");
        }

        for (uint16_t i = 0; i < amount; i++)
        {
            std::shared_ptr<DMX::Fixture> fix = std::make_shared<DMX::Fixture>(fixture);

            fix->id = fid;
            m_fixtures[fid] = fix;
            m_usedFids.insert(fid++);
            m_universes[universe].addFixture(fix, start);
        }
    }

    void addFixtureGroup(const DMX::FixtureGroup& fixtureGroup)
    {
        m_groups.push_back(fixtureGroup);
    }

    void addToGroup(uint16_t index, const std::vector<uint16_t>& fids)
    {
        // TODO: check index
        auto group = m_groups[index];
        for (auto& fid : fids)
        {
            group += m_fixtures[fid];
        }
    }

    DMX::Universe& getUniverse(uint8_t universe)
    {
        return m_universes[universe];
    }

    DMX::FixtureGroup& getFixtureGroup(uint8_t index)
    {
        return m_groups[index];
    }
};
