//
// Created by bobi on 25/09/2025.
//

#pragma once
#include<fstream>
#include <string>
#include <unordered_map>

#include "DMX/Fixture/Fixture.h"

class FixtureLibrary
{
    std::unordered_map<std::string, DMX::Fixture> m_fixtures;
public:

    void add(const DMX::Fixture& fix)
    {
        m_fixtures[fix.name] = fix;
    }

    void add(DMX::Fixture&& fixture)
    {
        // m_fixtures[]
        // m_fixtures.insert(fixture.name, fixture);
        m_fixtures[fixture.name] = std::move(fixture);
    }

    void readFromFile(const std::string& path)
    {
        std::ifstream file;
        file.open(path);

        std::string line;
        while (std::getline(file, line))
        {

        }
    }

    std::optional<DMX::Fixture> get(const std::string& name)
    {
        if (m_fixtures.contains(name))
        {
            return m_fixtures[name];
        }

        return std::nullopt;
    }
};
