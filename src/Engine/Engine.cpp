//
// Created by bobi on 24/09/2025.
//

#include "Engine/Engine.h"

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
        m_fixtures[fid] = fix;
        m_usedFids.insert(fid++);
        m_universes[universe].addFixture(fix, start);
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
    m_fixtures[newFID] = std::move(m_fixtures[currentFID]);
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

DMX::FixtureGroup& Engine::getFixtureGroup(const std::string& name)
{
    return m_groups[name];
}

std::vector<std::shared_ptr<DMX::Parameters::Parameter>>& Engine::getGroupParameter(const std::string& name, DMX::Parameters::ParameterTypes paramType)
{
    return getFixtureGroup(name).getParameters(paramType);
}