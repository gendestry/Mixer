//
// Created by bobi on 13/08/2025.
//

#include "../../src/DMX/Preset.h"

#pragma once

namespace DMX
{
struct Presets : public Traits::Printable
{
    std::vector<Preset> m_presets;

    template <typename TPreset>
    Presets(TPreset&& presets)
        : m_presets(std::forward<TPreset>(presets))
    {}

    Preset& operator[] (size_t index);

    [[nodiscard]] std::string describe() const noexcept override;
};
}