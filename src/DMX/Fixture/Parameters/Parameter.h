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

enum class Type
{
    NONE = 0x0,
    DIMMER = 1U << 0U,
    COLOR = 1U << 1U,
    POSITION = 1U << 2U,
};


class Parameter : public Traits::Printable
{
protected:
    // public:
    uint8_t* m_bytes = nullptr;
    uint32_t m_baseOffset;
    Type m_type;
    std::size_t m_size;
    uint8_t m_bytes_per_type;
    std::unordered_map<std::string, uint16_t> m_offsets;
    std::unordered_map<std::string, uint16_t> m_updatedOffset;
    std::vector<std::string> m_names;


    [[nodiscard]] static std::string parameterTypesToString(Type t);
    [[nodiscard]] static std::string fieldTypeToString(FieldTypes field);
public:
    Parameter(uint8_t* bytes, uint32_t baseOffset);
    explicit Parameter(uint8_t* uni, uint32_t baseOffset, uint8_t bytesPerType, Type type, std::size_t size);

    [[nodiscard]] Type getType() const;
    [[nodiscard]] std::size_t getSize() const;

    void setBuffer(uint8_t* bytes);
    void setOffset(uint32_t offset)
    {
        m_baseOffset = offset;
    }

    void setValue(const std::string& key, float percentage);
    [[nodiscard]] uint8_t* getBuffer() const {return m_bytes;}

    [[nodiscard]] std::string describe() const override;
};
}

