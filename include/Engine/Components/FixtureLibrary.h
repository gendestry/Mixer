//
// Created by bobi on 25/09/2025.
//

#pragma once
#include<fstream>
#include <ranges>
#include <string>
#include <unordered_map>
#include <format>

#include "DMX/Fixture/Fixture.h"

namespace Components {
    class FixtureLibrary : public Traits::Printable
    {
        std::unordered_map<std::string, DMX::Fixture> m_fixtures;

        static bool addFixtureParameter(DMX::Fixture& fixture, const std::string name)
        {
            if (name == "RGB")
            {
                fixture.add<DMX::Parameters::ColorRGB>();
            }
            else if (name == "Dimmer")
            {
                fixture.add<DMX::Parameters::Dimmer>();
            }
            else
            {
                return false;
            }

            return true;

        }
    public:

        void add(const DMX::Fixture& fix);
        void add(DMX::Fixture&& fixture);
        void readFromFile(const std::string& path);

        std::optional<DMX::Fixture> get(const std::string& name)
        {
            if (m_fixtures.contains(name))
            {
                return m_fixtures[name];
            }

            return std::nullopt;
        }

        [[nodiscard]] std::string describe() const override
        {
            std::stringstream ss;
            ss << "[Fixture Library]: " << m_fixtures.size() << " in library" << std::endl;
            for (const auto&[key, value] : m_fixtures)
            {
                ss << " - " << value.describe() << std::endl;
            }

            return ss.str();
        }
    };
}