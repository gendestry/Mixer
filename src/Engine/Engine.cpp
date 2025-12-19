//
// Created by bobi on 24/09/2025.
//

#include "Engine/Engine.h"
#include <iostream>

#include "DMX/Fixture/Parameters/VDimmerParameter.h"

Engine::Engine(const std::string& filePath)
{
    m_fixtureLibrary.readFromFile(filePath);
}

std::vector<uint16_t> Engine::patch(const std::string& fixtureName, uint8_t universe, uint16_t amount, std::optional<uint16_t> start, std::optional<uint16_t> startFID)
{
    auto fixture = m_fixtureLibrary.get(fixtureName);
    if (fixture.has_value())
    {
        return patch(fixture.value(), universe, amount, start, startFID);
    }

    throw std::runtime_error("Could not find fixture " + fixtureName);
}

std::vector<uint16_t> Engine::patch(DMX::Fixture& fixture, uint8_t universe, uint16_t amount, std::optional<uint16_t> start, std::optional<uint16_t> startFID)
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
        m_universes[universe].addFixture(fix, start);
        m_universes[universe].setID(universe);
        m_fixturesByName[fix->name].push_back(fix);
    }

    if(!m_senders.contains(universe))
    {
        SacnSender* sender = new SacnSender(universe);
        sender->setBuffer(m_universes[universe].getBytes());
        m_senders[universe] = sender;

    }

    return ret;
}

void Engine::addToGroup(const std::string& name, const std::vector<uint16_t>& fids)
{
    // TODO: check index
    auto& group = m_groups[name];
    for (auto fid : fids)
    {
        group += m_fixtures[fid];
    }
}

void Engine::setFixtureID(uint16_t currentFID, uint16_t newFID)
{
    m_usedFids.erase(currentFID);
    m_usedFids.insert(newFID);
    m_fixtures[currentFID]->id = newFID;
    m_fixtures[newFID] = m_fixtures[currentFID];
    // m_fixtures[currentFID
}

void Engine::addDirtyUniverse(std::string group)
{
    const auto& groupUni = m_groups[group].getUsedUniverses();
    m_dirtyUniverses.insert(groupUni.begin(), groupUni.end());
}

void Engine::clearDirtyUniverses()
{
    m_dirtyUniverses.clear();
}

void Engine::update()
{
    for(auto& d : m_dirtyUniverses)
    {
        m_senders[d]->send();
    }
}



std::vector<std::shared_ptr<DMX::Fixture>>& Engine::getFixturesByName(const std::string& name)
{
    return m_fixturesByName[name];
}

std::vector<uint16_t> Engine::getFixturesFIDByName(const std::string& name)
{
    auto& fixs = getFixturesByName(name);
    std::vector<uint16_t> ret;
    for (auto& fix : fixs)
    {
        ret.push_back(fix->id);
    }

    return ret;
}

// void removeFromGroup(const std::string& name, const std::vector<uint16_t>& fids)
// {
//     auto& group = m_groups[name];
//     for (auto fid : fids)
//     {
//
//     }
// }

DMX::Universe& Engine::getUniverse(uint8_t universe)
{
    return m_universes[universe];
}

std::shared_ptr<DMX::Fixture> Engine::getFixtureByFID(uint16_t fid)
{
    for (auto& [key, value] : m_fixtures)
    {
        if (value->id == fid)
        {
            return value;
        }
    }

    return nullptr;
}


DMX::FixtureGroup& Engine::getFixtureGroup(const std::string& name)
{
    return m_groups[name];
}

std::vector<std::shared_ptr<DMX::Parameters::Parameter>>& Engine::getGroupParameter(const std::string& name, DMX::Parameters::Type paramType)
{
    return getFixtureGroup(name).getParameters(paramType);
}

std::string Engine::describe() const
{
    std::stringstream ss;
    ss << m_fixtureLibrary.describe();

    for (auto& [key, value] : m_groups)
    {
        ss << "Fixture group: " << key << std::endl;
        const auto& fixs = value.get();
        for (auto fix : fixs)
        {
            ss << " - " << fix->describe() << std::endl;
        }
        ss << std::endl;
    }
    ss << "\n[Universes]" << std::endl;
    for (const auto& [_, value] : m_universes)
    {
        ss << value.describe();
    }
    return ss.str();
}
