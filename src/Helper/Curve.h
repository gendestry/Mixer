//
// Created by bobi on 06/10/2025.
//

#pragma once
#include <cstdint>
#include <math.h>
#include <stdexcept>
#include <vector>

#include "Utils.h"

namespace Utils
{

class Curve
{
    std::vector<float> m_values;
    std::size_t m_length;
public:
    enum Type
    {
        SINUSOID,
        TRIANGLE,
        SQUARE,
        LINE
    };

    explicit Curve(Type type, uint16_t length, uint16_t peaks = 1U)
        : m_length(length)
    {
        m_values.resize(length);
        auto temp = Utils::linearInterpolation(0, 1, length);

        for (auto i = 0; i < length; i++)
        {
            double lin = temp[i];
            double phase = peaks * lin;
            switch (type)
            {
            case SINUSOID:
                m_values[i] = std::sin(phase * M_PI);
                break;
            case TRIANGLE:
                double t = std::fmod(phase * 2.0, 2.0);
                m_values[i] = (t <= 1.0) ? t : 2.0 - t;
                break;
            case SQUARE:
                m_values[i] = std::fmod(phase, 1.0) < 0.5 ? 1.0 : 0.0;
                break;
            case LINE:

            default:
                throw std::invalid_argument("Invalid curve type");
            }
        }
    }

    float operator[] (std::size_t index) const
    {
        return m_values[index % m_length];
    }
};
}
