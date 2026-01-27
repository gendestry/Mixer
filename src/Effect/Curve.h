//
// Created by bobi on 06/10/2025.
//

#pragma once
#include <cstdint>
#include <math.h>
#include <stdexcept>
#include <vector>

#include "Helper/Utils.h"

namespace Utils
{

class Curve
{
    std::vector<float> m_values;
    std::size_t m_length;
    uint16_t m_peaks = 1;
public:
    enum Type
    {
        SINUSOID,
        TRIANGLE,
        SQUARE,
        LINE
    } m_type;

    Curve() = default;
    // explicit Curve(Type type, uint16_t length, uint16_t peaks = 1U);

    void setType(Type type) { m_type = type; }
    void setLength(std::size_t length) { m_length = length; }
    void setPeaks(uint16_t peaks) { m_peaks = peaks; }
    void update();
    // void init(Type type, uint16_t length, uint16_t peaks = 1U);

    float operator[] (std::size_t index) const;
};
}
