#include "Curve.h"
#include "Helper/Utils.h"
#include <math.h>
#include <stdexcept>

namespace Utils
{
    Curve::Curve(Type type, uint16_t length, uint16_t peaks)
    : m_type(type), m_length(length), m_peaks(peaks)
    {
        update();
    //     init(type, length, peaks);
    }

    void Curve::update()
    {
        m_values.resize(m_length);
        auto temp = Utils::linearInterpolation(0, 1, m_length);

        for (auto i = 0; i < m_length; i++)
        {
            double lin = temp[i];
            double phase = m_peaks * lin;
            switch (m_type)
            {
            case Type::SINUSOID:
            {
                constexpr double TWO_PI = M_PI;
                m_values[i] = std::fabs(std::sin(lin * m_peaks * TWO_PI));
                break;
            }
            case Type::TRIANGLE:
            {
                double t = std::fmod(phase * 2.0, 2.0);
                m_values[i] = (t <= 1.0) ? t : 2.0 - t;
                break;
            }
            case Type::SQUARE:
            {
                m_values[i] = std::fmod(phase, 1.0) < 0.5 ? 1.0 : 0.0;
                break;
            }
            case Type::LINE:
            {
                break;
            }

            default:
            {
                throw std::invalid_argument("Invalid curve type");
            }
            }
        }
    }

    float Curve::operator[] (std::size_t index) const
    {
        return m_values[index % m_length];
    }
}