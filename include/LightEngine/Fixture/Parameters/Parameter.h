#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>

#include "Descriptor.h"
#include "Utils/Colors/RGB.h"
#include "Utils/Traits/Stringify.h"

//
// Parameter: a typed *view* over a slice of the universe DMX buffer. It never
// owns bytes - it holds a buffer pointer + base offset and a pointer to a shared
// (runtime, interned) Descriptor that says where each channel lives. One
// concrete type; the Descriptor, not the C++ type, says what it is.
//
// All values crossing the API are normalized floats in [0, 1].
//
namespace LightEngine::Parameters
{
    class Parameter : public Utils::Traits::Stringify
    {
    protected:
        const Descriptor* m_desc       = nullptr;  // shared, runtime (flyweight)
        uint8_t*          m_bytes      = nullptr;   // -> universe buffer (non-owning)
        uint32_t          m_baseOffset = 0;         // start of this param in the buffer

        // 16-bit DMX is big-endian: coarse byte first, fine byte second.
        void writeNorm(const Channel& c, float norm);
        [[nodiscard]] float readNorm(const Channel& c) const;

    public:
        Parameter() = default;
        explicit Parameter(const Descriptor* desc) : m_desc(desc) {}
        ~Parameter() override = default;

        // Polymorphic deep copy - lets a Fixture be copied (and placed) without
        // its parameters being shared between instances.
        [[nodiscard]] virtual std::shared_ptr<Parameter> clone() const;

        void setDescriptor(const Descriptor* desc) { m_desc = desc; }
        void setBuffer(uint8_t* bytes)             { m_bytes = bytes; }
        void setOffset(uint32_t offset)            { m_baseOffset = offset; }

        [[nodiscard]] Type     getType() const { return m_desc ? m_desc->type : Type::NONE; }
        [[nodiscard]] uint16_t getSize() const { return m_desc ? m_desc->size : 0U; }
        [[nodiscard]] uint8_t* getBuffer() const { return m_bytes; }
        [[nodiscard]] const Descriptor* descriptor() const { return m_desc; }
        [[nodiscard]] bool has(Field f) const { return m_desc && m_desc->has(f); }

        // ---- generic indexed channel access (no hashing; no-op if absent) ----
        void setChannel(Field f, float norm);
        void multiplyChannel(Field f, float factor);
        [[nodiscard]] std::optional<float> getChannel(Field f) const;

        // ---- semantic helpers (operate on whatever fields the descriptor has) ----
        void setColor(const Utils::Colors::RGB& c);
        [[nodiscard]] Utils::Colors::RGB getColor() const;
        void applyIntensity(float intensity);

        void setLevel(float norm)             { setChannel(Field::DIMMER, norm); }
        [[nodiscard]] float getLevel() const  { return getChannel(Field::DIMMER).value_or(0.0f); }

        void setPan(float norm)  { setChannel(Field::PAN, norm); }
        void setTilt(float norm) { setChannel(Field::TILT, norm); }
        void setPanTilt(float pan, float tilt) { setPan(pan); setTilt(tilt); }

        std::string toString() override;
    };
}
