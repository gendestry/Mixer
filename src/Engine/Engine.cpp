//
// Created by bobi on 24/09/2025.
//

#include "Engine/Engine.h"
#include <iostream>

#include "DMX/Fixture/Parameters/VDimmerParameter.h"

Engine::Engine()
{
    m_patch.readFixtureLib("testfile.txt");
}

Engine::Engine(const std::string &filePath)
{
    m_patch.readFixtureLib(filePath);
}

void Engine::setIP(std::string ip)
{
    m_output.setIP(ip);
}

std::vector<uint16_t> Engine::patch(const std::string &fixtureName, uint8_t universe, uint16_t amount, std::optional<uint16_t> start, std::optional<uint16_t> startFID)
{
    auto ret = m_patch.patch(fixtureName, universe, amount, start, std::move(startFID));
    m_output.check(universe, m_patch.getUniverse(universe).getBytes());
    return ret;
}

std::vector<uint16_t> Engine::patch(DMX::Fixture &fixture, uint8_t universe, uint16_t amount, std::optional<uint16_t> start, std::optional<uint16_t> startFID)
{
    auto ret = m_patch.patch(fixture, universe, amount, start, std::move(startFID));
    m_output.check(universe, m_patch.getUniverse(universe).getBytes());
    return ret;
}

void Engine::addToGroup(const std::string &name, const std::vector<uint16_t> &fids)
{
    // TODO: check index
    auto& fixs = m_patch.getFixtures();
    auto &group = m_groups[name];
    for (auto fid : fids)
    {
        group += fixs[fid];
    }
}

void Engine::addToGroup(const std::string &name, uint16_t startFid, uint16_t endFid)
{
    addToGroup(name, Utils::make_range(startFid, endFid));
}

void Engine::addToGroup(const std::string &name, const std::string &otherName)
{
    m_groups[name] += m_groups[otherName];
}

void Engine::addDirtyUniverse(std::string group)
{
    const auto &groupUni = m_groups[group].getUsedUniverses();
    m_dirtyUniverses.insert(groupUni.begin(), groupUni.end());
}

void Engine::clearDirtyUniverses()
{
    m_dirtyUniverses.clear();
}

void Engine::progColorEffect(std::string group)
{
}

void Engine::update()
{
    m_output.update(m_dirtyUniverses);
}

// void removeFromGroup(const std::string& name, const std::vector<uint16_t>& fids)
// {
//     auto& group = m_groups[name];
//     for (auto fid : fids)
//     {
//
//     }
// }

DMX::FixtureGroup &Engine::getFixtureGroup(const std::string &name)
{
    return m_groups[name];
}

std::vector<std::shared_ptr<DMX::Parameters::Parameter>> &Engine::getGroupParameter(const std::string &name, DMX::Parameters::Type paramType)
{
    return getFixtureGroup(name).getParameters(paramType);
}

std::vector<std::shared_ptr<DMX::Parameters::Parameter>> &Engine::getGroupColorParameter(const std::string &name)
{
    return getFixtureGroup(name).getParameters(DMX::Parameters::Type::COLOR);
}

std::string Engine::describe() const
{
    std::stringstream ss;
    for (auto &[key, value] : m_groups)
    {
        ss << "Fixture group: " << key << std::endl;
        const auto &fixs = value.get();
        for (auto fix : fixs)
        {
            ss << " - " << fix->describe() << std::endl;
        }
        ss << std::endl;
    }

    ss << m_patch.describe();
    return ss.str();
}
