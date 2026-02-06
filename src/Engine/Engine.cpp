//
// Created by bobi on 24/09/2025.
//

#include "Engine/Engine.h"

Engine::Engine()
{
    m_patch.readFixtureLib("testfile.txt");
}

Engine::Engine(const std::string &filePath)
{
    m_patch.readFixtureLib(filePath);
}

void Engine::setIP(const std::string& ip)
{
    m_output.setIP(ip);
}

std::vector<uint16_t> Engine::patch(const std::string &fixtureName, uint8_t universe, uint16_t amount, std::optional<uint16_t> start, std::optional<uint16_t> startFID)
{
    auto ret = m_patch.patch(fixtureName, universe, amount, start, startFID);
    m_output.check(universe, m_patch.getUniverse(universe).getBytes());
    return ret;
}

std::vector<uint16_t> Engine::patch(DMX::Fixture &fixture, uint8_t universe, uint16_t amount, std::optional<uint16_t> start, std::optional<uint16_t> startFID)
{
    auto ret = m_patch.patch(fixture, universe, amount, start, startFID);
    m_output.check(universe, m_patch.getUniverse(universe).getBytes());
    return ret;
}

void Engine::addToGroup(const std::string &name, const std::vector<uint16_t> &fids)
{
    // TODO: check index
    m_groupManager.addToGroup(name, m_patch.getFixturesByFIDS(fids));
    addDirtyUniverse(name);
}

void Engine::addToGroup(const std::string &name, uint16_t startFid, uint16_t endFid)
{
    addToGroup(name, Utils::make_range(startFid, endFid));
}

void Engine::addToGroup(const std::string &name, const std::string &otherName)
{
    m_groupManager.addToGroup(name, otherName);
    addDirtyUniverse(name);
}

void Engine::addDirtyUniverse(const std::string& group)
{
    const auto &groupUni = m_groupManager[group].getUsedUniverses();
    m_dirtyUniverses.insert(groupUni.begin(), groupUni.end());
}

void Engine::clearDirtyUniverses()
{
    m_dirtyUniverses.clear();
}

void Engine::update()
{
    for (Effect::Effect* effect : m_effects)
    {
        effect->update();
    }

    m_output.update(m_dirtyUniverses);

    // clearDirtyUniverses();
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
    return m_groupManager[name];
    // return m_groups[name];
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
    ss << m_groupManager.describe();

    ss << m_patch.describe();
    return ss.str();
}
