#pragma once

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include "LightEngine/Effects/EffectEngine.h"
#include "LightEngine/Effects/TimeContext.h"
#include "LightEngine/Engine/Components/Patch.h"
#include "LightEngine/Show/Cue.h"

//
// Sequence: an ordered list of cues (a playback list / executor). store() saves
// a look into a cue; go() advances to the next cue and instantiates its effects;
// apply() renders the active cue every frame. Effects recalled from a cue are
// built (once, on go) into a private EffectEngine via the EffectFactory.
//
namespace LightEngine::Show
{
    class Sequence
    {
        std::string             m_name;
        std::vector<Cue>        m_cues;
        int                     m_current = -1;       // index of active cue, -1 = none
        Effects::EffectEngine   m_playback;           // live effects for the active cue

        void instantiate(LightEngine::Engine::Components::Patch& patch);

    public:
        Sequence() = default;
        explicit Sequence(std::string name) : m_name(std::move(name)) {}

        [[nodiscard]] const std::string& name() const { return m_name; }
        [[nodiscard]] std::size_t size() const { return m_cues.size(); }
        [[nodiscard]] bool active() const { return m_current >= 0; }

        Cue& store(const Cue& cue);                              // add or replace by number
        void go(LightEngine::Engine::Components::Patch& patch);         // advance to next cue
        void goTo(std::size_t index, LightEngine::Engine::Components::Patch& patch);

        void apply(const Effects::TimeContext& t, LightEngine::Engine::Components::Patch& patch);

        [[nodiscard]] std::string describe() const;
    };
}
