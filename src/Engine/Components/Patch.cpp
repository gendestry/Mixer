//
// Created by bobi on 27. 01. 26.
//

#include "Engine/Components/Patch.h"
#include <sstream>
namespace Components {
    void Patch::readFixtureLib(const std::string& filepath)
    {
        m_fixtureLibrary.readFromFile(filepath);
    }

    std::vector<uint16_t> Patch::patch(DMX::Fixture &fixture, uint8_t universe, uint16_t amount, std::optional<uint16_t> address, std::optional<uint16_t> startFID)
    {
        uint16_t fid = startFID.has_value() ? startFID.value() : universe * 100 + 1;
        std::vector<uint16_t> ret;
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

            ret.push_back(fid);
            fix->id = fid;
            fix->m_universe = universe;
            m_fixtures[fid] = fix;
            m_usedFids.insert(fid++);
            m_universes[universe].addFixture(fix, address);
            m_universes[universe].setID(universe);
            m_fixturesByName[fix->name].push_back(fix);
        }

        // if (!m_senders.contains(universe))
        // {
        //     SacnSender *sender = new SacnSender(universe, m_ip.c_str());
        //     sender->setBuffer(m_universes[universe].getBytes());
        //     m_senders[universe] = sender;
        // }

        return ret;
    }

    std::vector<uint16_t> Patch::patch(const std::string &fixtureName, uint8_t universe, uint16_t amount, std::optional<uint16_t> address, std::optional<uint16_t> startFID)
    {
        auto fixture = m_fixtureLibrary.get(fixtureName);
        if (fixture.has_value())
        {
            return patch(fixture.value(), universe, amount, address, startFID);
        }

        throw std::runtime_error("Could not find fixture " + fixtureName);
    }

    void Patch::setFixtureID(uint16_t currentFID, uint16_t newFID)
    {
        m_usedFids.erase(currentFID);
        m_usedFids.insert(newFID);
        m_fixtures[currentFID]->id = newFID;
        m_fixtures[newFID] = m_fixtures[currentFID];
        // m_fixtures[currentFID
    }

    std::map<uint16_t, std::shared_ptr<DMX::Fixture>>& Patch::getFixtures()
    {
        return m_fixtures;
    }

    DMX::Universe &Patch::getUniverse(uint8_t universe)
    {
        return m_universes[universe];
    }

    std::shared_ptr<DMX::Fixture> Patch::getFixtureByFID(uint16_t fid)
    {
        for (auto &[key, value] : m_fixtures)
        {
            if (value->id == fid)
            {
                return value;
            }
        }

        return nullptr;
    }

    std::vector<std::shared_ptr<DMX::Fixture>> &Patch::getFixturesByName(const std::string &name)
    {
        return m_fixturesByName[name];
    }

    std::vector<uint16_t> Patch::getFixturesFIDByName(const std::string &name)
    {
        auto &fixs = getFixturesByName(name);
        std::vector<uint16_t> ret;
        for (auto &fix : fixs)
        {
            ret.push_back(fix->id);
        }

        return ret;
    }

    std::string Patch::describe() const
    {
        std::stringstream ss;
        ss << m_fixtureLibrary.describe();

        // for (auto &[key, value] : m_groups)
        // {
        //     ss << "Fixture group: " << key << std::endl;
        //     const auto &fixs = value.get();
        //     for (auto fix : fixs)
        //     {
        //         ss << " - " << fix->describe() << std::endl;
        //     }
        //     ss << std::endl;
        // }
        ss << "\n[Universes]" << std::endl;
        for (const auto &[_, value] : m_universes)
        {
            ss << value.describe();
        }
        return ss.str();
    }
}
