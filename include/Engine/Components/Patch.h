//
// Created by bobi on 27. 01. 26.
//

#pragma once
#include <vector>
#include <cstdint>
#include "DMX/Universe.h"
#include "DMX/Fixture/Fixture.h"
#include "FixtureLibrary.h"
#include <memory>
#include <map>
#include <set>

namespace Components
{
    class Patch
    {
        FixtureLibrary m_fixtureLibrary;

        std::set<uint8_t> m_dirtyUniverses;
        std::map<uint16_t, DMX::Universe> m_universes;
        std::map<uint16_t, std::shared_ptr<DMX::Fixture>> m_fixtures;
        std::map<std::string, std::vector<std::shared_ptr<DMX::Fixture>>> m_fixturesByName;
        std::set<uint16_t> m_usedFids;
    public:

        void readFixtureLib(const std::string& filepath);

        std::vector<uint16_t> patch(DMX::Fixture &fixture, uint8_t universe, uint16_t amount, std::optional<uint16_t> address = 0, std::optional<uint16_t> startFID = 0);
        std::vector<uint16_t> patch(const std::string &fixtureName, uint8_t universe, uint16_t amount, std::optional<uint16_t> address = 0, std::optional<uint16_t> startFID = 0);

        void setFixtureID(uint16_t currentFID, uint16_t newFID);

        [[nodiscard]] std::map<uint16_t, std::shared_ptr<DMX::Fixture>>& getFixtures();
        [[nodiscard]] DMX::Universe &getUniverse(uint8_t universe);
        [[nodiscard]] std::shared_ptr<DMX::Fixture> getFixtureByFID(uint16_t fid);
        [[nodiscard]] std::vector<std::shared_ptr<DMX::Fixture>> &getFixturesByName(const std::string &name);
        [[nodiscard]] std::vector<uint16_t> getFixturesFIDByName(const std::string &name);

        std::string describe() const;

    };
}