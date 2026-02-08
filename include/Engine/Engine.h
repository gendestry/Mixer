//
// Created by bobi on 24/09/2025.
//

#pragma once
#include <cstdint>
#include <set>
#include <chrono>

#include "DMX/FixtureGroup.h"
#include "Components/DMXOutput.h"
#include "Components/Patch.h"
#include "Components/GroupManager.h"

#include "Effect/Effect.h"

class Engine : public Traits::Printable
{
    std::set<uint8_t> m_dirtyUniverses;
    std::vector<Effect::Effect*> m_effects;
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

    // void addEffectColor(const std::string& groupName, const std::function<void(uint32_t, std::vector<std::shared_ptr<DMX::Parameters::Parameter>>&)>& func)
    // {
    //     Effect::Effect* effect = new Effect::ColorEffect(getFixtureGroup(groupName));
    //     effect->setEffect(func);
    //     m_effects.push_back(effect);
    // }

    Effect::ColorEffect* addColor(const std::string& groupName, Utils::Colors::RGB color)
    {
        Effect::Effect* effect = new Effect::ColorEffect(getFixtureGroup(groupName), color);
        m_effects.push_back(effect);
        return static_cast<Effect::ColorEffect*>(effect);
    }

    Effect::DimmerEffect* addDimmer(const std::string& groupName, float percentage)
    {
        Effect::Effect* effect = new Effect::DimmerEffect(getFixtureGroup(groupName), percentage);
        m_effects.push_back(effect);
        return static_cast<Effect::DimmerEffect*>(effect);
    }

    Effect::FXDimmerGrouped* addEffectDimmerGroup(const std::string& groupName, Utils::Curve::Type curve = Utils::Curve::SINUSOID)
    {
        Effect::Effect* effect = new Effect::FXDimmerGrouped(getFixtureGroup(groupName), curve);
        m_effects.push_back(effect);
        return static_cast<Effect::FXDimmerGrouped*>(effect);
    }

    Effect::FXDimmerChase* addEffectDimmerChase(const std::string& groupName, Utils::Curve::Type curve = Utils::Curve::SINUSOID)
    {
        Effect::Effect* effect = new Effect::FXDimmerChase(getFixtureGroup(groupName), curve);
        m_effects.push_back(effect);
        return static_cast<Effect::FXDimmerChase*>(effect);
    }

    // Effect::FXDimmerChase* addEffectDimmerChase(const std::string& groupName, Utils::CurveVariant&& curve)
    // {
    //     Effect::Effect* effect = new Effect::FXDimmerChase(getFixtureGroup(groupName), std::move(curve));
    //     m_effects.push_back(effect);
    //     return static_cast<Effect::FXDimmerChase*>(effect);
    // }

    Effect::FX2Color* addEffect2Color(const std::string& groupName, Utils::Colors::RGB color1, Utils::Colors::RGB color2)
    {
        Effect::Effect* effect = new Effect::FX2Color(getFixtureGroup(groupName), color1, color2);
        m_effects.push_back(effect);
        return static_cast<Effect::FX2Color*>(effect);
    }

    Effect::FXColorGradient* addEffectColorGradient(const std::string& groupName, const std::vector<Utils::Colors::RGB>& colors, const std::vector<float>& percentages)
    {
        Effect::Effect* effect = new Effect::FXColorGradient(getFixtureGroup(groupName), colors, percentages);
        m_effects.push_back(effect);
        return static_cast<Effect::FXColorGradient*>(effect);
    }

    void update();

    [[nodiscard]] Components::Patch &getPatch() { return m_patch;}
    [[nodiscard]] Components::GroupManager &groups() { return m_groupManager;}

    [[nodiscard]] DMX::FixtureGroup &getFixtureGroup(const std::string &name);
    [[nodiscard]] std::vector<std::shared_ptr<DMX::Parameters::Parameter>> &getGroupParameter(const std::string &name, DMX::Parameters::Type paramType);
    [[nodiscard]] std::vector<std::shared_ptr<DMX::Parameters::Parameter>> &getGroupColorParameter(const std::string &name);

    [[nodiscard]] std::string describe() const override;
};
