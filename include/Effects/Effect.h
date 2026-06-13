#pragma once

#include "DMX/FixtureGroup.h"
#include "Effects/TimeContext.h"

//
// Effect: a unit of behaviour bound (non-owning) to a FixtureGroup, re-evaluated
// every frame. apply() writes the effect's contribution into the group's
// fixtures. Effects are run in insertion order by the EffectEngine; later
// effects override earlier ones for the same parameter type (LTP), while
// different parameter types compose.
//
namespace Core::Effects
{
    class Effect
    {
    protected:
        DMX::FixtureGroup* m_group;
        bool               m_enabled = true;

    public:
        explicit Effect(DMX::FixtureGroup& group) : m_group(&group) {}
        virtual ~Effect() = default;

        virtual void apply(const TimeContext& t) = 0;

        void setEnabled(bool enabled) { m_enabled = enabled; }
        [[nodiscard]] bool enabled() const { return m_enabled; }
        [[nodiscard]] DMX::FixtureGroup& group() const { return *m_group; }
    };
}
