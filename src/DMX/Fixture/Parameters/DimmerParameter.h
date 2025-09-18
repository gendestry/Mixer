//
// Created by bobi on 15/09/2025.
//

#pragma once
#include "Parameter.h"
#include <cmath>


namespace DMX::Parameters
{
    template<uint8_t TBytes, DimmerType... TTypes>
    struct DimmerParameter final : public Parameter
    {
        static constexpr std::size_t k_size = sizeof...(TTypes) * TBytes;

        DimmerParameter(uint8_t* bytes, uint16_t baseOffset)
            : Parameter(bytes, baseOffset, TBytes, ParameterTypes::DIMMER, k_size)
        {
            int16_t index = -TBytes;
            ((m_offsets[fieldTypeToString(TTypes)] = index += TBytes), ...);

        }

    };
}
