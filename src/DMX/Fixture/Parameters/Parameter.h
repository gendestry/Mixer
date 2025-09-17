#pragma once
#include <string>
#include <variant>
#include <unordered_map>
#include <cstdint>

#include "Helper/Printable.h"

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
    uint16_t m_baseOffset = 0U;
    ParameterTypes m_type;
    std::size_t m_size;
    uint8_t m_bytes_per_type;
    std::unordered_map<std::string, uint16_t> m_offsets;


    [[nodiscard]] static std::string parameterTypesToString(ParameterTypes t);
    [[nodiscard]] static std::string fieldTypeToString(FieldTypes field);
public:
    Parameter(uint8_t* bytes, uint16_t baseOffset);
    explicit Parameter(uint8_t* uni, uint16_t baseOffset, uint8_t bytesPerType, ParameterTypes type, std::size_t size);

    [[nodiscard]] ParameterTypes getType() const;
    [[nodiscard]] std::size_t getSize() const;

    void setBuffer(uint8_t* bytes);
    void setValue(const std::string& key, float percentage);

    [[nodiscard]] std::string describe() const override;
};
}

