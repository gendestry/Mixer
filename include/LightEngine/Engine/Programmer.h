#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "Utils/Colors/RGB.h"
#include "Utils/Math/Curve.h"

#include "LightEngine/Attributes/Preset.h"
#include "LightEngine/DMX/FixtureGroup.h"
#include "LightEngine/Effects/EffectEngine.h"
#include "LightEngine/Effects/EffectSpec.h"
#include "LightEngine/Effects/Effects.h"
#include "LightEngine/Fixture/Values.h"
#include "LightEngine/Show/Cue.h"

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
namespace LightEngine::Engine
{
    class Programmer
    {
        using FixturePtr = std::shared_ptr<LightEngine::Fixture>;

        std::vector<FixturePtr>            m_selection;
        bool                               m_latched = false;   // edited since last select()
        std::map<FixturePtr, LightEngine::Values> m_values;            // accumulated live values
        std::set<FixturePtr>               m_driven;            // every fixture the programmer touches
        Effects::EffectEngine              m_effects;
        std::vector<int>                   m_selectedGroups;    // ids of pooled Groups currently lit

        void touch() { m_latched = true; }
        [[nodiscard]] DMX::FixtureGroup selectionGroup() const;

    public:
        // ---- selection ----
        void select(DMX::FixtureGroup& group);          // fresh selection (or continue if not latched)
        void addToSelection(DMX::FixtureGroup& group);
        void clearSelection();
        [[nodiscard]] const std::vector<FixturePtr>& selection() const { return m_selection; }
        [[nodiscard]] std::vector<uint16_t> selectionFids() const;

        // Which pooled Groups are currently part of the selection (UI highlight).
        void addSelectedGroup(int id) { m_selectedGroups.push_back(id); }
        [[nodiscard]] const std::vector<int>& selectedGroups() const { return m_selectedGroups; }

        // ---- values on the current selection ----
        void setColor(const Utils::Colors::RGB& color);
        void setIntensity(float level);

        // ---- effects on the current selection (built via the factory) ----
        Effects::Effect* addEffect(Effects::Spec spec);
        Effects::DimmerChase* addDimmerChase(float bpm, Utils::Maths::Type type = Utils::Maths::SINUSOID);

        // ---- palette authoring on the current selection ----
        // Fan a colour a->b across the ordered selection (per-fixture gradient).
        void applyColorGradient(const Utils::Colors::RGB& a, const Utils::Colors::RGB& b);
        // Stamp a pooled preset onto the current selection (Absolute or ByIndex).
        void applyPreset(const Attributes::Preset& preset);

        // ---- snapshots for storing into pools ----
        // Per-FID values for one feature (the Preset payload). Skips fixtures
        // with nothing set for that feature.
        [[nodiscard]] std::map<uint16_t, LightEngine::Values> snapshot(Attributes::Feature feature) const;
        // Recipes of the live effects (the FxPreset payload).
        [[nodiscard]] std::vector<Effects::Spec> effectSpecs() const;

        // ---- store the current programmer state into a cue ----
        [[nodiscard]] Show::Cue makeCue(float number) const;

        // ---- lifecycle ----
        void apply(const Effects::TimeContext& t);   // render the programmer this frame
        void clear();                                // wipe everything (the Clear button)

        [[nodiscard]] bool empty() const;
        [[nodiscard]] std::string describe() const;
    };
}
