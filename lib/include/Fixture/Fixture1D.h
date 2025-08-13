#pragma once

#include <format>

#include "Fixture.h"

namespace Fixture
{

FixtureTemplate
class Fixture1D final : public Fixture<TOrder, TPin>
{
    uint16_t m_numGroups;

public:
    Fixture1D(const std::vector<DMX::Preset>& presets, const DMX::DMXInfo& info, uint16_t numLeds)
        : Fixture<TOrder, TPin>(numLeds, presets, info)
    {
        updatePreset();
    }

    void update() override {

        uint8_t* dmxBuffer = this->m_sacn->getDMXBuffer();
        uint16_t fixturesInGroup = this->m_numLeds / m_numGroups;
        uint16_t ledIndex = 0;

        for(uint16_t groupInDMX = 0; groupInDMX < m_numGroups; groupInDMX++)
        {
            for(uint16_t fixtureInGroup = 0; fixtureInGroup < fixturesInGroup; fixtureInGroup++)
            {
                for(uint8_t byte = 0; byte < this->k_bytesPerLed; byte++)
                {
                    uint16_t dmxBufferIndex = this->m_dmxInfo.getAddress() + groupInDMX * this->k_bytesPerLed + byte;
                    this->m_ledBytes[ledIndex] = dmxBuffer[dmxBufferIndex];
                    ledIndex++;
                }
            }
        }

    }

    void updatePreset() override
    {
        // if()
        m_numGroups = this->m_presets[this->m_dmxInfo.getPresetIndex()].m_settings[0];
    };



    [[nodiscard]] std::string describe() const noexcept override
    {
        return std::format("[Fixture1D] {} leds  =>  {} groups of {} pixels\n{}",
            this->m_numLeds,
            (this->m_numLeds / m_numGroups),
            this->m_numGroups,
            this->getLightDescription());
    };

};

};
