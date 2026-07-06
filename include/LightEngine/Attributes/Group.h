#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "LightEngine/Attributes/Attribute.h"

//
// Group: a stored, poolable selection of fixtures - the "who". It holds FIDs
// (not live fixtures) so it is stable across re-patching and serializable. A
// group carries no values; that separation is what lets you select a group and
// then stamp a preset onto it. Recall it by resolving the FIDs through the Patch
// into a live FixtureGroup.
//
namespace LightEngine::Attributes
{
    class Group : public Attribute
    {
        std::vector<uint16_t> m_fids;   // ordered, like the programmer selection

    public:
        Group() = default;
        explicit Group(std::string name) : Attribute(std::move(name)) {}
        Group(std::string name, std::vector<uint16_t> fids)
            : Attribute(std::move(name)), m_fids(std::move(fids)) {}

        [[nodiscard]] const std::vector<uint16_t>& fids() const { return m_fids; }
        void setFids(std::vector<uint16_t> fids) { m_fids = std::move(fids); }
        [[nodiscard]] bool empty() const { return m_fids.empty(); }

        [[nodiscard]] std::string describe() const override
        {
            return "Group \"" + name() + "\" [" + std::to_string(m_fids.size()) + " fixtures]";
        }
    };
}
