#include "Curve.h"

#include <iostream>

#include "Helper/Utils.h"
#include <math.h>
#include <ostream>
#include <stdexcept>

namespace Utils {
    namespace Curve {
        Interface::Interface(Type type, uint16_t length, uint16_t peaks)
        : m_type(type), m_length(length), m_peaks(peaks)
        {
            // update();
        //     init(type, length, peaks);
        }

        Interface::Interface(Type type, float min, float max, uint16_t length, uint16_t peaks)
        : m_type(type), m_min(min / 100.f), m_max(max / 100.f), m_length(length), m_peaks(peaks)
        {
            // update();
            //     init(type, length, peaks);
        }

        void Interface::init() {
            fill();
        }

        void Interface::update()
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

        float Interface::operator[] (std::size_t index) const
        {
            return m_values[index % m_length];
        }

        void Sinusoid::fill() {
            m_values.clear();
            m_values.resize(m_length);

            const auto lin = Utils::linearInterpolation(0, 1, m_length);
            for (auto i = 0; i < m_length; i++)
            {
                const double phase = m_peaks * lin[i];
                const double val = std::fabs(std::sin(phase * M_PI));
                m_values[i] = m_min + val * (m_max - m_min);
            }
        }

        void Triangle::fill() {
            m_values.clear();
            m_values.resize(m_length);

            const auto lin = Utils::linearInterpolationMirrored(m_min, m_max, m_length/m_peaks);
            const auto linlen = lin.size();

            for (int p = 0; p < m_peaks; p++)
            {
                for (auto i = 0; i < linlen; i++)
                {
                    m_values[p * linlen + i] = lin[i];
                }
            }
        }

        void Square::fill() {
            m_values.clear();
            m_values.resize(m_length);
            const auto lin = Utils::linearInterpolation(0, 1, m_length);

            for (auto i = 0; i < m_length; i++)
            {
                double phase = m_peaks * lin[i];
                double m = std::fmod(phase, 1.f);
                m_values[i] = (m < 0.5) ? m_max : m_min;
            }
        }
    }

}