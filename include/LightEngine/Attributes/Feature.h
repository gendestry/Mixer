#pragma once

#include <cstdint>
#include <string_view>

#include "LightEngine/Fixture/Values.h"

//
// Feature: which family of attributes a Preset covers. It doubles as a mask over
// the Values struct - a Color preset only carries the .color field, an Intensity
// preset only .intensity. This is what lets you stack Color then Intensity onto
// the same fixtures without one clobbering the other.
//
// NOTE: Values currently models only color + intensity, so Position is a
// recognised feature but not yet applyable (marked TODO below) until Values
// grows pan/tilt.
//
namespace LightEngine::Attributes
{
    enum class Feature : uint8_t
    {
        Color,
        Intensity,
        Position,
        Dimmer,      // alias of Intensity for now (real dimmer channel)
    };

    // How a preset re-applies to a selection on recall.
    enum class RecallMode : uint8_t
    {
        Absolute,    // per stored FID: fixture N always gets its stored value
        ByIndex,     // fan stored values across the selection by position (reusable)
    };

    [[nodiscard]] inline std::string_view featureName(Feature f)
    {
        switch (f)
        {
            case Feature::Color:     return "Color";
            case Feature::Intensity: return "Intensity";
            case Feature::Position:  return "Position";
            case Feature::Dimmer:    return "Dimmer";
        }
        return "?";
    }

    // Keep only the fields of `v` that belong to feature `f`.
    [[nodiscard]] inline Values maskFeature(const Values& v, Feature f)
    {
        Values out;
        switch (f)
        {
            case Feature::Color:
                out.color = v.color;
                break;
            case Feature::Intensity:
            case Feature::Dimmer:
                out.intensity = v.intensity;
                break;
            case Feature::Position:
                break;   // TODO: pan/tilt once Values carries position
        }
        return out;
    }

    // Overlay the feature-relevant field of `src` onto `dst` (only if set).
    inline void mergeFeature(Values& dst, const Values& src, Feature f)
    {
        switch (f)
        {
            case Feature::Color:
                if (src.color) dst.color = src.color;
                break;
            case Feature::Intensity:
            case Feature::Dimmer:
                if (src.intensity) dst.intensity = src.intensity;
                break;
            case Feature::Position:
                break;   // TODO: pan/tilt once Values carries position
        }
    }

    // Does `v` carry any value for feature `f`?
    [[nodiscard]] inline bool hasFeature(const Values& v, Feature f)
    {
        switch (f)
        {
            case Feature::Color:     return v.color.has_value();
            case Feature::Intensity:
            case Feature::Dimmer:    return v.intensity.has_value();
            case Feature::Position:  return false;
        }
        return false;
    }
}
