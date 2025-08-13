//
// Created by bobi on 13/08/2025.
//
#pragma once
#include <vector>
#include <cstdint>

namespace Traits
{
    class Connectable
    {

        std::array<uint8_t, 512U> m_dmxBuffer{0};
        // uint8_t m_universe;

        // explicit Connectable(uint8_t universe)
        //     : m_universe(universe)
        // {};
        //
        // virtual ~Connectable() = default;
    public:
        virtual void connect(uint8_t universe) = 0;
        virtual void disconnect() = 0;
        [[nodiscard]] inline uint8_t* getDMXBuffer() { return this->m_dmxBuffer.data(); }
    };
}