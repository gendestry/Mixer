//
// Created by bobi on 24/09/2025.
//

#pragma once
#include <cstdint>
#include <set>
#include <map>

#include "../../src/DMX/FixtureGroup.h"
#include "../../src/DMX/Universe.h"

class Engine
{
    std::unordered_map<uint8_t, DMX::Universe> m_universes;
    std::map<std::string, DMX::FixtureGroup> m_groups;
    std::map<uint16_t, std::shared_ptr<DMX::Fixture>> m_fixtures;
    std::set<uint16_t> m_usedFids;

public:

    std::vector<uint16_t> patch(DMX::Fixture& fixture, uint8_t universe, uint16_t amount, std::optional<uint16_t> start = std::nullopt, std::optional<uint16_t> startFID = std::nullopt);
    void addToGroup(const std::string& name, const std::vector<uint16_t>& fids);
    void setFixtureID(uint16_t currentFID, uint16_t newFID);

    [[nodiscard]] DMX::Universe& getUniverse(uint8_t universe);
    [[nodiscard]] DMX::FixtureGroup& getFixtureGroup(const std::string& name);

    [[nodiscard]] std::vector<std::shared_ptr<DMX::Parameters::Parameter>>& getGroupParameter(const std::string& name, DMX::Parameters::ParameterTypes paramType);
};
