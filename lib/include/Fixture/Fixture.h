#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include <memory>
#include <concepts>

#include "Traits/Printable.h"
#include "DMXInput/sACN_manager.h"
#include "DMX/Presets.h"
#include "../../src/DMX/DMXInfo.h"
#include "DMXOutput/Outputable.h"

namespace Fixture
{

// #define FixtureTemplate template<DMX::Output::LedOrder TOrder, uint8_t TPin>
// using DmxBuffer = std::array<uint8_t, 512>;
// #define TFixtureClass F

#define FixtureTemplate template<DMX::Output::LedOrder TOrder, uint8_t TPin>
// #define FixtureTemplate template<uint16_t TNumLeds, DMX::Output::LedOrder TOrder \
//     template<typename...> class TWrapper]>
// #define FixtureCTemplated Fixture<TNumLeds, TOrder, TWrapper>
#define FixtureCTemplated Fixture<TOrder, TPin>



FixtureTemplate
class Fixture : public Traits::Printable
{
protected:
    const uint8_t k_bytesPerLed = DMX::Output::OrderToNum<TOrder>::amount;
    uint16_t m_numLeds;
    std::vector<uint8_t> m_ledBytes;
    // std::unique_ptr<DMX::Output::OutputWrapper<TNumLeds, TOrder>> m_output;
    std::shared_ptr<DMX::Input::sACN> m_sacn = nullptr;
    DMX::Presets m_presets;
    DMX::DMXInfo m_dmxInfo;

    virtual void updatePreset() = 0;

public:
    Fixture(uint16_t numLeds, const std::vector<DMX::Preset>& presets, const DMX::DMXInfo& info);
    virtual ~Fixture() = default;

    virtual void update() = 0;
    void setUniverse(uint8_t universe);
    void setAddress(uint16_t address);
    void setPreset(uint8_t preset);

    [[nodiscard]] std::string getLightDescription() const;

};

FixtureTemplate
FixtureCTemplated::Fixture(uint16_t numLeds, const std::vector<DMX::Preset>& presets, const DMX::DMXInfo& info)
    : m_numLeds(numLeds), m_presets(presets), m_dmxInfo(info), m_sacn(DMX::Input::sACNManager::get().getsACN(m_dmxInfo.getUniverse()))
{
    setUniverse(info.getUniverse());
    setAddress(info.getAddress());
    setPreset(info.getPresetIndex());
    m_ledBytes.resize(numLeds * k_bytesPerLed);
    // m_sacn->connect(m_dmxInfo.getUniverse());
}

FixtureTemplate
void FixtureCTemplated::setUniverse(uint8_t universe)
{
    if(this->m_dmxInfo.getUniverse() != universe)
    {
        this->m_dmxInfo.setUniverse(universe);
        // delete m_sacn;
        m_sacn.reset();
    }

    m_sacn = DMX::Input::sACNManager::get().getsACN(universe);
}

FixtureTemplate
void FixtureCTemplated::setAddress(uint16_t address)
{
    this->m_dmxInfo.setAddress(address);
}

FixtureTemplate
void FixtureCTemplated::setPreset(uint8_t preset)
{
    this->m_dmxInfo.setPresetIndex(preset);
    // updatePreset();
}

FixtureTemplate
std::string FixtureCTemplated::getLightDescription() const
{
    std::stringstream ss;
    // std::format(" -")
    ss << " -" << this->m_dmxInfo.describe() << "\n"
       << " -" << this->m_presets.describe();

    return ss.str();
}

template<template<DMX::Output::LedOrder, uint8_t> class TLedDerived, DMX::Output::LedOrder TLedOrder, uint8_t TPin, typename... Args>
requires std::derived_from<TLedDerived<TLedOrder, TPin>, Fixture<TLedOrder, TPin>>
std::unique_ptr<Fixture<TLedOrder, TPin>> make_fixture(Args&&... args)
{
    return std::make_unique<TLedDerived<TLedOrder, TPin>>(std::forward<Args>(args)...);
};

};

