#pragma once

#include "Fixture.h"

namespace Fixture
{

FixtureTemplate
class Fixture2D final : public Fixture<TOrder, TPin>
{
    uint16_t m_width;
    uint16_t m_height;
    uint16_t m_rows;
    uint16_t m_columns;

    std::unordered_map<uint32_t, std::vector<uint16_t>> m_hash;

public:
    Fixture2D(const std::vector<DMX::Preset>& presets, const DMX::DMXInfo& info, const uint16_t width, const uint16_t height)
        : Fixture<TOrder, TPin>(width * height, presets, info), m_width(width), m_height(height)
    {
        updatePreset();
    }

    void update() override
    {
        uint8_t* dmxBuffer = this->m_sacn->getDMXBuffer();
        for(uint16_t y = 0; y < m_rows; y++)
        {
            for(uint16_t x = 0; x < m_columns; x++)
            {
                const auto& indexes = getGridIndexes(x, y);
                uint16_t dmxIndex = this->m_dmxInfo.getAddress() + (x * this->k_bytesPerLed) + (y * this->k_bytesPerLed) * m_columns;

                for(auto& index : indexes)
                {
                    for(uint8_t byte = 0; byte < this->k_bytesPerLed; byte++)
                    {
                        this->m_ledBytes[index * this->k_bytesPerLed + byte] = dmxBuffer[dmxIndex + byte];
                    }
                }
            }
        }
    }

    const std::vector<uint16_t>& getGridIndexes(uint16_t x, uint16_t y) {
        // TODO: Find bigger prime numbers or better m_hashing
        uint32_t hnum = x * 7741 + y * 7757;

        if(m_hash.contains(hnum)) {
            return m_hash[hnum];
        }

        const uint16_t hsize = m_height / m_rows;
        const uint16_t wsize = m_width / m_columns;

        for(uint16_t yi = y * hsize; yi < (y + 1) * hsize; yi++) {
            for(uint16_t xi = x * wsize; xi < (x + 1) * wsize; xi++) {
                m_hash[hnum].push_back(xi + yi * m_width);
            }
        }

        return m_hash[hnum];
    }

    void updatePreset() override
    {
        m_columns = this->m_presets[this->m_dmxInfo.getPresetIndex()].m_settings[0];
        m_rows = this->m_presets[this->m_dmxInfo.getPresetIndex()].m_settings[1];
    };


    [[nodiscard]] std::string describe() const noexcept override
    {
        return std::format("[Fixture2D] width: {}, height: {}, rows: {}, columns: {}\n{}",
            this->m_width, this->m_height, m_rows, m_columns, this->getLightDescription());
    };

};

};
