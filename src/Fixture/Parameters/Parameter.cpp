#include "Fixture/Parameters/Parameter.h"

#include <algorithm>

namespace Core::Parameters
{
    std::shared_ptr<Parameter> Parameter::clone() const
    {
        return std::make_shared<Parameter>(*this);
    }

    void Parameter::writeNorm(const Channel& c, float norm)
    {
        norm = std::clamp(norm, 0.0f, 1.0f);
        uint8_t* p = m_bytes + m_baseOffset + c.offset;
        if (c.width == 2)
        {
            const auto v = static_cast<uint16_t>(norm * 65535.0f + 0.5f);
            p[0] = static_cast<uint8_t>(v >> 8U);
            p[1] = static_cast<uint8_t>(v & 0xFFU);
        }
        else
        {
            p[0] = static_cast<uint8_t>(norm * 255.0f + 0.5f);
        }
    }

    float Parameter::readNorm(const Channel& c) const
    {
        const uint8_t* p = m_bytes + m_baseOffset + c.offset;
        if (c.width == 2)
            return ((static_cast<uint16_t>(p[0]) << 8U) | p[1]) / 65535.0f;
        return p[0] / 255.0f;
    }

    void Parameter::setChannel(Field f, float norm)
    {
        if (m_desc) if (const Channel* c = m_desc->find(f)) writeNorm(*c, norm);
    }

    void Parameter::multiplyChannel(Field f, float factor)
    {
        if (m_desc) if (const Channel* c = m_desc->find(f)) writeNorm(*c, readNorm(*c) * factor);
    }

    std::optional<float> Parameter::getChannel(Field f) const
    {
        if (m_desc) if (const Channel* c = m_desc->find(f)) return readNorm(*c);
        return std::nullopt;
    }

    void Parameter::setColor(const Utils::Colors::RGB& c)
    {
        setChannel(Field::R, c.r / 255.0f);
        setChannel(Field::G, c.g / 255.0f);
        setChannel(Field::B, c.b / 255.0f);
    }

    Utils::Colors::RGB Parameter::getColor() const
    {
        return {
            static_cast<uint8_t>(getChannel(Field::R).value_or(0.0f) * 255.0f),
            static_cast<uint8_t>(getChannel(Field::G).value_or(0.0f) * 255.0f),
            static_cast<uint8_t>(getChannel(Field::B).value_or(0.0f) * 255.0f),
        };
    }

    void Parameter::applyIntensity(float intensity)
    {
        multiplyChannel(Field::R, intensity);
        multiplyChannel(Field::G, intensity);
        multiplyChannel(Field::B, intensity);
        multiplyChannel(Field::W, intensity);
    }

    std::string Parameter::toString()
    {
        std::string s{typeName(getType())};
        s += " @" + std::to_string(m_baseOffset) + " [" + std::to_string(getSize()) + "B]";
        if (m_desc)
            for (const auto& c : m_desc->channels)
                s += "\n  - " + std::string(fieldName(c.field))
                   + " +" + std::to_string(c.offset)
                   + " (" + std::to_string(c.width * 8) + "bit)";
        return s;
    }
}
