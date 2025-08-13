//
// Created by bobi on 13/08/2025.
//

#include "DMX/Presets.h"
#include <sstream>

namespace DMX
{
    Preset& Presets::operator[] (size_t index)
    {
        return m_presets[index];
    }

    [[nodiscard]] std::string Presets::describe() const noexcept
    {
        std::stringstream ss;
        ss << "Presets: {\n";
        for (const auto& p : m_presets)
        {
            ss << " \t" << p.describe() << "\n";
        }
        ss << "}";
        return ss.str();
    }
}