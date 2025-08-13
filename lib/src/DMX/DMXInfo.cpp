#include "DMXInfo.h"

#include <format>

namespace DMX
{
    DMXInfo::DMXInfo(uint8_t universe, uint16_t address, uint8_t presetIndex)
            : m_universe(universe), m_address(address), m_presetIndex(presetIndex)
    {}

    void DMXInfo::setUniverse(uint8_t universe) { m_universe = universe; }
    void DMXInfo::setAddress(uint16_t address) { m_address = address; }
    void DMXInfo::setPresetIndex(uint8_t presetIndex) { m_presetIndex = presetIndex; }

    uint8_t DMXInfo::getUniverse() const { return m_universe; }
    uint16_t DMXInfo::getAddress() const { return m_address; }
    uint8_t DMXInfo::getPresetIndex() const { return m_presetIndex; }


    std::string DMXInfo::describe() const noexcept
    {
        return std::format("DMX: {{ Universe: {}, Address: {}, Current preset: {} }}",
            m_universe,
            m_address,
            m_presetIndex);
        // ss << "DMX: { Universe: " << std::to_string(m_universe)
        // << ", Address: " << std::to_string(m_address)
        // << ", PresetIndex: " << std::to_string(m_presetIndex);
        // return std::format("{", m_universe, m_address, m_presetIndex);
    };
};