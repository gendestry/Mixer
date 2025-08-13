//
// Created by bobi on 13/08/2025.
//

#include "Preset.h"

namespace DMX
{
    std::string Preset::describe() const noexcept
    {
        std::stringstream ss;
        ss << m_description << ": { ";

        const uint16_t size = m_settings.size();
        for (uint16_t index = 0; index < size; index++)
        {
            const auto& setting = m_settings.at(index);
            ss << std::to_string(setting);

            if (index + 1U < size)
            {
                ss << ", ";
            }
        }

        ss << " }";
        return ss.str();
    }

}