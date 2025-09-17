//
// Created by bobi on 17/09/2025.
//

#include "Parameter.h"
#include <stdexcept>
#include <sstream>
#include <cmath>

namespace DMX::Parameters
{
    Parameter::Parameter(uint8_t* bytes, uint16_t baseOffset)
        : m_type(ParameterTypes::NONE), m_bytes(bytes), m_baseOffset(baseOffset), m_size(0)
    {}

    Parameter::Parameter(uint8_t* uni, uint16_t baseOffset, uint8_t bytesPerType, ParameterTypes type, std::size_t size)
        : m_bytes(uni), m_baseOffset(baseOffset), m_bytes_per_type(bytesPerType), m_type(type), m_size(size)
    {}

    // [[nodiscard]] uint16_t getOffset (FieldTypes type) const
    // {
    //     const std::string name = fieldTypeToString(type);
    //
    //     if (m_offsets.contains(name))
    //     {
    //         return m_offsets.at(name) + m_baseOffset;
    //     }
    //
    //     throw std::out_of_range("Color does not exist");
    // }

    ParameterTypes Parameter::getType() const { return m_type; }
    std::size_t Parameter::getSize() const { return m_size; }

    void Parameter::setBuffer(uint8_t* bytes)
    {
        m_bytes = bytes;
    }

    void Parameter::setValue(const std::string& key, float percentage)
    {
        uint32_t size = pow(2, m_bytes_per_type * 8);
        uint32_t p = static_cast<uint32_t>((percentage / 100.0f) * size);

        if (m_offsets.contains(key))
        {
            const auto off = m_baseOffset + m_offsets[key];
            if (m_bytes_per_type == 1)
            {
                // printf("%p\n", &m_bytes[m_offsets[key]]);
                // printf("%d\n", m_offsets[key]);
                m_bytes[off] = static_cast<uint8_t>(p);
            }
        }
    }

    std::string Parameter::describe() const
    {
        std::stringstream ss;
        ss << "[" << parameterTypesToString(m_type) << "]\n";
        for (auto& [key, value] : m_offsets)
        {
            ss << " - " << key << ": " << (value + m_baseOffset) << std::endl;
            // ss << " - " << key << ": " << (value) << std::endl;
        }

        return ss.str();
    }

    std::string Parameter::parameterTypesToString(ParameterTypes t)
    {
        switch (t)
        {
        case ParameterTypes::NONE:
            return "NONE";
        case ParameterTypes::DIMMER:
            return "DIMMER";
        case ParameterTypes::COLOR:
            return "COLOR";
        case ParameterTypes::POSITION:
            return "POSITION";
        default:
            return "UNKNOWN";
            // throw std::invalid_argument("Invalid parameter type");
        }
    }

    std::string Parameter::fieldTypeToString(FieldTypes field)
    {
        return std::visit([](auto&& arg) -> std::string
        {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, ColorType>) {
                // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
                switch (arg)
                {
                    case ColorType::R:
                    return "R";
                case ColorType::G:
                    return "G";
                case ColorType::B:
                    return "B";
                case ColorType::W:
                    return "W";
            }
        }
        else if constexpr (std::is_same_v<T, PositionType>)
        {
            // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
            switch (arg)
            {
                case PositionType::PAN:
                return "PAN";
            case PositionType::TILT:
                return "TILT";
        }
    }
    else if constexpr (std::is_same_v<T, DimmerType>)
    {
        // ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
        switch (arg)
        {
            case (DimmerType::DIMMER):
            return "DIMMER";
    }
}

return "UNKNOWN";
}, field);
    }
}