#pragma once

#include <string>
#include <utility>
#include <vector>

#include "LightEngine/Attributes/Attribute.h"
#include "LightEngine/Effects/EffectSpec.h"

//
// FxPreset: a stored, poolable set of effect recipes. Unlike value presets it
// stores Effects::Spec (kind + FID targets + parameters), exactly like a Cue
// stores its effects, so recall goes through the EffectFactory to build live
// effects. This is the FX pool payload.
//
namespace LightEngine::Attributes
{
    class FxPreset : public Attribute
    {
        std::vector<Effects::Spec> m_specs;

    public:
        FxPreset() = default;
        explicit FxPreset(std::string name) : Attribute(std::move(name)) {}

        [[nodiscard]] const std::vector<Effects::Spec>& specs() const { return m_specs; }
        void setSpecs(std::vector<Effects::Spec> specs) { m_specs = std::move(specs); }
        [[nodiscard]] bool empty() const { return m_specs.empty(); }

        [[nodiscard]] std::string describe() const override
        {
            return "FxPreset \"" + name() + "\" [" + std::to_string(m_specs.size()) + " effects]";
        }
    };
}
