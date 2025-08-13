#pragma once
#include <cstdint>
#include "Traits/Printable.h"

namespace DMX
{
    class DMXInfo : public Traits::Printable
    {
    public:
        DMXInfo(uint8_t universe, uint16_t address, uint8_t presetIndex);

        void setUniverse(uint8_t universe);
        void setAddress(uint16_t address);
        void setPresetIndex(uint8_t presetIndex);

        [[nodiscard]] uint8_t getUniverse() const;
        [[nodiscard]] uint16_t getAddress() const;
        [[nodiscard]] uint8_t getPresetIndex() const;

        [[nodiscard]] std::string describe() const noexcept override;

    private:
        uint8_t m_universe;
        uint16_t m_address;
        uint8_t m_presetIndex;
    };
};