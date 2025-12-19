//
// Created by bobi on 24/09/2025.
//

#pragma once
#include <cstdint>
#include <set>
#include <map>

#include "FixtureLibrary.h"
#include "DMX/FixtureGroup.h"
#include "DMX/Universe.h"
#include "Helper/SacnSender.h"

class Engine : public Traits::Printable
{
    std::set<uint8_t> m_dirtyUniverses;
    std::map<uint16_t, DMX::Universe> m_universes;
    std::map<uint16_t, SacnSender*> m_senders;
    std::map<std::string, DMX::FixtureGroup> m_groups;
    std::map<uint16_t, std::shared_ptr<DMX::Fixture>> m_fixtures;
    std::map<std::string, std::vector<std::shared_ptr<DMX::Fixture>>> m_fixturesByName;
    std::set<uint16_t> m_usedFids;

    FixtureLibrary m_fixtureLibrary;

public:
    Engine(const std::string& filePath);
    std::vector<uint16_t> patch(const std::string& fixtureName, uint8_t universe, uint16_t amount, std::optional<uint16_t> start = std::nullopt, std::optional<uint16_t> startFID = std::nullopt);
    std::vector<uint16_t> patch(DMX::Fixture& fixture, uint8_t universe, uint16_t amount, std::optional<uint16_t> start = std::nullopt, std::optional<uint16_t> startFID = std::nullopt);
    void addToGroup(const std::string& name, const std::vector<uint16_t>& fids);
    void setFixtureID(uint16_t currentFID, uint16_t newFID);
    void addDirtyUniverse(std::string group);
    void clearDirtyUniverses();
    void update();


    [[nodiscard]] DMX::Universe& getUniverse(uint8_t universe);
    [[nodiscard]] std::shared_ptr<DMX::Fixture> getFixtureByFID(uint16_t fid);
    [[nodiscard]] DMX::FixtureGroup& getFixtureGroup(const std::string& name);
    [[nodiscard]] std::vector<std::shared_ptr<DMX::Fixture>>& getFixturesByName(const std::string& name);
    [[nodiscard]] std::vector<uint16_t> getFixturesFIDByName(const std::string& name);

    [[nodiscard]] std::vector<std::shared_ptr<DMX::Parameters::Parameter>>& getGroupParameter(const std::string& name, DMX::Parameters::Type paramType);

    [[nodiscard]] std::string describe() const override;
};
