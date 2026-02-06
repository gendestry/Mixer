//
// Created by bobi on 06/10/2025.
//

#pragma once
#include <cstdint>
#include <vector>

namespace Utils
{

class Curve
{
    std::vector<float> m_values;
    uint32_t m_length;
    uint16_t m_peaks = 1;

    void update();
public:
    enum Type
    {
        SINUSOID,
        TRIANGLE,
        SQUARE,
        LINE
    } m_type;

    Curve() = delete;//default;
    explicit Curve(Type type, uint16_t length, uint16_t peaks = 1U);

    void setType(const Type type) { m_type = type; update(); }
    void setLength(const uint32_t length) { m_length = length; update();}
    void setPeaks(const uint16_t peaks) { m_peaks = peaks; update();}
    // void init(Type type, uint16_t length, uint16_t peaks = 1U);

    float operator[] (std::size_t index) const;
};
}
