//
// Created by bobi on 15/09/2025.
//

#pragma once
#include "Parameter.h"

namespace DMX::Parameters
{
    template<uint8_t TBytes, ColorType... TTypes>
    struct ColorParameter final : public Parameter
    {
        static constexpr std::size_t k_size = sizeof...(TTypes) * TBytes;

        ColorParameter(uint8_t* bytes, uint32_t baseOffset)
            : Parameter(bytes, baseOffset, TBytes, ParameterTypes::COLOR, k_size)
        {
            int16_t index = -TBytes;
            ((m_offsets[fieldTypeToString(TTypes)] = index += TBytes), ...);

        }

    };
}