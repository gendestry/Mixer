#pragma once

#include <cstdint>
#include <deque>
#include <initializer_list>
#include <vector>

#include "Channel.h"

//
// Descriptor: the runtime layout of one parameter, shared across every instance
// of the same fixture personality (flyweight). Built from parsed fixture-library
// data or from a preset; never mutated after interning.
//
namespace LightEngine::Parameters
{
    struct Descriptor
    {
        Type                 type = Type::NONE;
        uint16_t             size = 0;   // total bytes spanned
        std::vector<Channel> channels;

        Descriptor() = default;
        Descriptor(Type t, uint16_t sz, std::vector<Channel> chans);

        // Contiguous fields, all the same width (the common case).
        static Descriptor make(Type type, uint8_t width, std::initializer_list<Field> fields);

        // Explicit channels (mixed widths / non-contiguous, e.g. from a file).
        static Descriptor fromChannels(Type type, std::vector<Channel> chans);

        [[nodiscard]] const Channel* find(Field f) const;
        [[nodiscard]] bool has(Field f) const { return find(f) != nullptr; }

        bool operator==(const Descriptor&) const = default;
    };

    // Interns descriptors so identical layouts are shared (flyweight). The
    // fixture library owns one of these; std::deque keeps element addresses
    // stable as descriptors are added, so Parameter pointers never dangle.
    class DescriptorStore
    {
        std::deque<Descriptor> m_descriptors;

    public:
        const Descriptor* intern(const Descriptor& desc);
        [[nodiscard]] std::size_t size() const { return m_descriptors.size(); }
    };
}
