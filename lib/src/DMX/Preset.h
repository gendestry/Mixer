#pragma once
#include <string>
#include <vector>
#include <cstdint>

#include "Traits/Printable.h"

namespace DMX
{

struct Preset : public Traits::Printable
{
    std::vector<uint16_t> m_settings;
    std::string m_description;

    Preset(const std::vector<uint16_t>& settings, const std::string& description)
        : m_settings(settings), m_description(description)
    {}

    Preset(const std::vector<uint16_t>&& settings, const std::string&& description)
        : m_settings(std::move(settings)), m_description(std::move(description))
    {}

    // template <typename TSettings, typename TDescription>
    // Preset(TSettings&& settings, TDescription&& description)
    //     : m_settings(std::forward<TSettings>(settings)),
    //       m_description(std::forward<TDescription>(description))
    // {}
            // m_description(std))


    [[nodiscard]] std::string describe() const noexcept override;
    // Preset(std::vector<uint16_t> settings, std::string description)
    //     : m_settings(settings), m_description(description)
    // {}
};

};

