#pragma once

#include <cstdint>
#include <string_view>

//
// Low-level vocabulary shared by all parameters: what kind of attribute a
// parameter is (Type), the semantic role of a single DMX channel (Field), and
// the physical placement of one channel (Channel).
//
namespace LightEngine::Parameters
{
    // Capability flags - a fixture can be queried by what it supports
    // (e.g. COLOR | DIMMER). Powers of two so they OR together.
    enum class Type : uint8_t
    {
        NONE     = 0x0,
        DIMMER   = 1U << 0U,
        COLOR    = 1U << 1U,
        POSITION = 1U << 2U,
        VDIMMER  = 1U << 3U,
    };

    constexpr Type operator|(Type a, Type b) { return Type(uint8_t(a) | uint8_t(b)); }
    constexpr Type operator&(Type a, Type b) { return Type(uint8_t(a) & uint8_t(b)); }
    constexpr bool any(Type t) { return uint8_t(t) != 0U; }

    // The semantic role of a single physical DMX channel.
    enum class Field : uint8_t
    {
        R, G, B, W, A,   // color components
        PAN, TILT,       // position
        DIMMER           // intensity
    };

    // One logical channel: its role, byte offset relative to the parameter base,
    // and width (1 = 8-bit, 2 = 16-bit big-endian coarse/fine). Mixed widths in
    // one descriptor are allowed.
    struct Channel
    {
        Field    field;
        uint16_t offset;
        uint8_t  width;

        bool operator==(const Channel&) const = default;
    };

    [[nodiscard]] std::string_view typeName(Type t);
    [[nodiscard]] std::string_view fieldName(Field f);
}
