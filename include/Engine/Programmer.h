#pragma once

#include <list>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "Utils/Colors/RGB.h"
#include "Utils/Math/Curve.h"

#include "DMX/FixtureGroup.h"
#include "Effects/EffectEngine.h"
#include "Effects/Effects.h"

//
// Programmer: the live editing layer, like grandMA3 / ChamSys. You build a
// selection, then set values or add effects on it; those land in the programmer
// and accumulate (per fixture) until cleared or stored to a cue. The programmer
// is rendered every frame and takes precedence over playback.
//
// Selection semantics: selecting after you've set a value/effect starts a fresh
// selection (the values you already set stay in the programmer); consecutive
// selects before any edit add to the selection.
//
namespace Core::Engine
{
    class Programmer
    {
        using FixturePtr = std::shared_ptr<Core::Fixture>;

        struct State
        {
            std::optional<Utils::Colors::RGB> color;
            std::optional<float>              intensity;
        };

        std::vector<FixturePtr>      m_selection;
        bool                         m_latched = false;   // a value/effect was set since last select()
        std::map<FixturePtr, State>  m_values;            // accumulated live values, per fixture
        std::list<DMX::FixtureGroup> m_effectGroups;      // selection snapshots (stable refs for effects)
        Effects::EffectEngine        m_effects;

        void touch() { m_latched = true; }
        DMX::FixtureGroup& snapshotSelection();

    public:
        // ---- selection ----
        void select(DMX::FixtureGroup& group);          // fresh selection (or continue if not latched)
        void addToSelection(DMX::FixtureGroup& group);
        void clearSelection();
        [[nodiscard]] const std::vector<FixturePtr>& selection() const { return m_selection; }

        // ---- values on the current selection ----
        void setColor(const Utils::Colors::RGB& color);
        void setIntensity(float level);

        // ---- effects on the current selection (bound to a snapshot of it) ----
        template<typename T, typename... Args>
        T* addEffect(Args&&... args)
        {
            DMX::FixtureGroup& g = snapshotSelection();
            T* fx = m_effects.emplace<T>(g, std::forward<Args>(args)...);
            touch();
            return fx;
        }
        Effects::DimmerChase* addDimmerChase(float bpm, Utils::Maths::Type type = Utils::Maths::SINUSOID);

        // ---- lifecycle ----
        void apply(const Effects::TimeContext& t);   // render the programmer this frame
        void clear();                                // wipe everything (the Clear button)

        [[nodiscard]] bool empty() const;
        [[nodiscard]] std::string describe() const;
    };
}
