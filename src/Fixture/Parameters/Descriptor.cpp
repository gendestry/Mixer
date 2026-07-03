#include "LightEngine/Fixture/Parameters/Descriptor.h"

#include <algorithm>
#include <utility>

namespace LightEngine::Parameters
{
    Descriptor::Descriptor(Type t, uint16_t sz, std::vector<Channel> chans)
        : type(t), size(sz), channels(std::move(chans)) {}

    Descriptor Descriptor::make(Type type, uint8_t width, std::initializer_list<Field> fields)
    {
        std::vector<Channel> chans;
        chans.reserve(fields.size());
        uint16_t off = 0;
        for (Field f : fields)
        {
            chans.push_back({f, off, width});
            off += width;
        }
        return Descriptor{type, off, std::move(chans)};
    }

    Descriptor Descriptor::fromChannels(Type type, std::vector<Channel> chans)
    {
        uint16_t end = 0;
        for (const auto& c : chans)
            end = std::max<uint16_t>(end, c.offset + c.width);
        return Descriptor{type, end, std::move(chans)};
    }

    const Channel* Descriptor::find(Field f) const
    {
        for (const auto& c : channels)
            if (c.field == f) return &c;
        return nullptr;
    }

    const Descriptor* DescriptorStore::intern(const Descriptor& desc)
    {
        for (const auto& d : m_descriptors)
            if (d == desc) return &d;
        m_descriptors.push_back(desc);
        return &m_descriptors.back();
    }
}
