#pragma once

#include <cstdint>
#include <utility>
#include <vector>

#include "DMX/FixtureGroup.h"
#include "Effects/EffectSpec.h"
#include "Effects/TimeContext.h"

//
// Effect: a unit of behaviour re-evaluated every frame. It OWNS its target
// FixtureGroup by value (a cheap bag of shared fixture references), so a
// factory-built effect is fully self-contained - no external group storage.
// apply() writes the effect's contribution; spec() reports its current
// description (for storing into a cue).
//
namespace Core::Effects
{
    class Effect
    {
    protected:
        DMX::FixtureGroup m_group;
        bool              m_enabled = true;

        // FIDs of the fixtures this effect targets (for building a Spec).
        [[nodiscard]] std::vector<uint16_t> targetFids() const
        {
            std::vector<uint16_t> fids;
            fids.reserve(m_group.fixtures().size());
            for (const auto& f : m_group.fixtures())
                fids.push_back(f->fid());
            return fids;
        }

    public:
        explicit Effect(DMX::FixtureGroup group) : m_group(std::move(group)) {}
        virtual ~Effect() = default;

        virtual void apply(const TimeContext& t) = 0;
        [[nodiscard]] virtual Spec spec() const = 0;

        void setEnabled(bool enabled) { m_enabled = enabled; }
        [[nodiscard]] bool enabled() const { return m_enabled; }
        [[nodiscard]] DMX::FixtureGroup& group() { return m_group; }
        [[nodiscard]] const DMX::FixtureGroup& group() const { return m_group; }
    };
}
