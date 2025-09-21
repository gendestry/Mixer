#pragma once
#include <string>
#include <variant>
#include <unordered_map>
#include <cstdint>
#include <vector>

#include "Helper/Printable.h"
#include <iostream>

namespace DMX::Parameters
{

enum class ColorType
{
    R, G, B, W, A
};

enum class PositionType
{
    PAN, TILT
};

enum class DimmerType
{
    DIMMER
};

using FieldTypes = std::variant<ColorType, PositionType, DimmerType>;

enum class ParameterTypes
{
    NONE,
    DIMMER,
    COLOR,
    POSITION,
};


class Parameter : public Traits::Printable
{
protected:
    // public:
    uint8_t* m_bytes = nullptr;
    uint32_t m_baseOffset;
    ParameterTypes m_type;
    std::size_t m_size;
    uint8_t m_bytes_per_type;
    std::unordered_map<std::string, uint16_t> m_offsets;
    std::unordered_map<std::string, uint16_t> m_updatedOffset;
    std::vector<std::string> m_names;


    [[nodiscard]] static std::string parameterTypesToString(ParameterTypes t);
    [[nodiscard]] static std::string fieldTypeToString(FieldTypes field);
public:
    Parameter(uint8_t* bytes, uint32_t baseOffset);
    explicit Parameter(uint8_t* uni, uint32_t baseOffset, uint8_t bytesPerType, ParameterTypes type, std::size_t size);

    // Parameter(const Parameter& other) {
    //     std::cout << "Copy Constructor" << std::endl;
    // }

    // Parameter(const Parameter& other)
    // {
    //     m_bytes = other.m_bytes;
    //     m_baseOffset = other.m_baseOffset;
    //     m_type = other.m_type;
    //     m_size = other.m_size;
    //     m_bytes_per_type = other.m_bytes_per_type;
    //     m_offsets = other.m_offsets;
    //     m_names = other.m_names;
    //
    // }

    [[nodiscard]] ParameterTypes getType() const;
    [[nodiscard]] std::size_t getSize() const;

    void setBuffer(uint8_t* bytes);
    void setOffset(uint32_t offset)
    {
        m_baseOffset = offset;
        // std::cout << "Set base offset: " << m_baseOffset << std::endl;
        // m_updatedOffset.clear();
        // for (auto& [key, value] : m_offsets)
        // {
        //     m_updatedOffset[key] = offset + value;
        // }
    }
    void setValue(const std::string& key, float percentage);
    uint8_t* getBuffer() {return m_bytes;}

    [[nodiscard]] std::string describe() const override;
};
}

