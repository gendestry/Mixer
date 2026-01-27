//
// Created by bobi on 15/09/2025.
//

#pragma once
#include "Parameter.h"
#include <array>
#include <cstring>

namespace DMX::Parameters
{
    template<uint8_t TBytes, ColorType... TTypes>
    struct ColorParameter final : public Parameter
    {
        static constexpr std::size_t k_size = sizeof...(TTypes) * TBytes;
        std::array<uint8_t, k_size * TBytes> m_original;

        ColorParameter(uint8_t* bytes, uint32_t baseOffset)
            : Parameter(bytes, baseOffset, TBytes, Type::COLOR, k_size)
        {
            int16_t index = -TBytes;
            ((m_offsets[fieldTypeToString(TTypes)] = index += TBytes), ...);

        }

        void applyDimmer(float perc)
        {
            memcpy(m_original.data(), m_bytes + m_baseOffset, TBytes * k_size);
            for(uint8_t i = 0; i < k_size; i++)
            {
                this->m_bytes[i] = perc * 255;
            }
        }

    };
}