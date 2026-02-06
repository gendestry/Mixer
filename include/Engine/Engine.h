//
// Created by bobi on 24/09/2025.
//

#pragma once
#include <cstdint>
#include <set>

#include "DMX/FixtureGroup.h"
#include "Components/DMXOutput.h"
#include "Components/Patch.h"
#include "Components/GroupManager.h"

class Engine : public Traits::Printable
{
    std::set<uint8_t> m_dirtyUniverses;

    Components::Patch m_patch;
    Components::DMXOutput m_output;
    Components::GroupManager m_groupManager;

public:
    Engine();
    Engine(const std::string &filePath);

    void setIP(const std::string& ip);
    std::vector<uint16_t> patch(const std::string &fixtureName, uint8_t universe, uint16_t amount, std::optional<uint16_t> start = std::nullopt, std::optional<uint16_t> startFID = std::nullopt);
    std::vector<uint16_t> patch(DMX::Fixture &fixture, uint8_t universe, uint16_t amount, std::optional<uint16_t> start = std::nullopt, std::optional<uint16_t> startFID = std::nullopt);

    void addToGroup(const std::string &name, const std::vector<uint16_t> &fids);
    void addToGroup(const std::string &name, uint16_t startFid, uint16_t endFid);
    void addToGroup(const std::string &name, const std::string &otherName);
    // void setFixtureID(uint16_t currentFID, uint16_t newFID);
    void addDirtyUniverse(const std::string& group);
    void clearDirtyUniverses();

    void progColorEffect(std::string group);

    void update();

    [[nodiscard]] Components::Patch &getPatch() { return m_patch;}
    [[nodiscard]] Components::GroupManager &groups() { return m_groupManager;}

    [[nodiscard]] DMX::FixtureGroup &getFixtureGroup(const std::string &name);
    [[nodiscard]] std::vector<std::shared_ptr<DMX::Parameters::Parameter>> &getGroupParameter(const std::string &name, DMX::Parameters::Type paramType);
    [[nodiscard]] std::vector<std::shared_ptr<DMX::Parameters::Parameter>> &getGroupColorParameter(const std::string &name);

    [[nodiscard]] std::string describe() const override;
};
