#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "LightEngine/Attributes/Attribute.h"
#include "LightEngine/Attributes/Feature.h"
#include "LightEngine/Fixture/Values.h"

//
// Preset: a stored, poolable "what" for one Feature. It is a per-FID palette -
// map<FID, Values> masked to the feature - so different fixtures can hold
// different values (group 1 blue, group 2 red), and a gradient authored across
// an ordered selection is just this same map with smoothly varying values.
//
//   Absolute recall - each stored FID gets its stored value back.
//   ByIndex  recall - the stored values are fanned across the current selection
//                     by position, so a gradient replays on any fixture count.
//
namespace LightEngine::Attributes
{
    class Preset : public Attribute
    {
        Feature                    m_feature = Feature::Color;
        RecallMode                 m_mode    = RecallMode::Absolute;
        std::map<uint16_t, Values> m_values;   // per-FID, already feature-masked

    public:
        Preset() = default;
        Preset(std::string name, Feature feature)
            : Attribute(std::move(name)), m_feature(feature) {}

        [[nodiscard]] Feature    feature() const { return m_feature; }
        [[nodiscard]] RecallMode mode() const    { return m_mode; }
        void setMode(RecallMode mode)            { m_mode = mode; }

        [[nodiscard]] const std::map<uint16_t, Values>& values() const { return m_values; }
        void setValues(std::map<uint16_t, Values> values) { m_values = std::move(values); }
        [[nodiscard]] bool empty() const { return m_values.empty(); }

        // Stored value for one FID (Absolute recall). Null if the preset has none.
        [[nodiscard]] const Values* valuesFor(uint16_t fid) const
        {
            const auto it = m_values.find(fid);
            return it != m_values.end() ? &it->second : nullptr;
        }

        // Stored values in FID-ascending order (map is ordered), for ByIndex fanning.
        [[nodiscard]] std::vector<Values> ordered() const
        {
            std::vector<Values> out;
            out.reserve(m_values.size());
            for (const auto& [fid, v] : m_values) out.push_back(v);
            return out;
        }

        [[nodiscard]] std::string describe() const override
        {
            return "Preset \"" + name() + "\" [" + std::string(featureName(m_feature))
                 + ", " + std::to_string(m_values.size()) + " fixtures]";
        }
    };
}
