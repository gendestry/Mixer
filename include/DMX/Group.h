//
// Created by Bobi on 9/22/25.
//

#pragma once
#include <memory>
#include <vector>
#include <list>

#include "Fixture/Fixture.h"

class FixtureGroup {
    std::vector<std::shared_ptr<DMX::Fixture>> m_fixtures;
public:
    void add(const std::shared_ptr<DMX::Fixture>& fixture)
    {
        m_fixtures.push_back(fixture);
    }

    void add(const std::list<std::shared_ptr<DMX::Fixture>>& fixs)
    {
        m_fixtures.insert(m_fixtures.end(), fixs.begin(), fixs.end());
    }

    const std::vector<std::shared_ptr<DMX::Fixture>>& get()
    {
        return m_fixtures;
    }

    size_t size() const
    {
        return m_fixtures.size();
    }


    FixtureGroup &operator=(const std::vector<std::shared_ptr<DMX::Fixture>> &fixtures)
    {
        this->m_fixtures = fixtures;
        return *this;
    }

    FixtureGroup &operator+=(const std::shared_ptr<DMX::Fixture>& fixture)
    {
        this->m_fixtures.push_back(fixture);
        return *this;
    }

    FixtureGroup &operator+=(const std::list<std::shared_ptr<DMX::Fixture>> &fixtures)
    {
        this->m_fixtures.insert(this->m_fixtures.end(), fixtures.begin(), fixtures.end());
        return *this;
    }

    FixtureGroup &operator+=(FixtureGroup &other)
    {
        auto otherLights = other();
        this->m_fixtures.insert(this->m_fixtures.end(), otherLights.begin(), otherLights.end());
        return *this;
    }

    FixtureGroup &operator-=(const std::list<std::shared_ptr<DMX::Fixture>> &lights)
    {
        this->m_fixtures.insert(this->m_fixtures.end(), lights.rbegin(), lights.rend());
        return *this;
    }

    FixtureGroup &operator-=(FixtureGroup &other)
    {
        auto otherLights = other();
        this->m_fixtures.insert(this->m_fixtures.end(), otherLights.rbegin(), otherLights.rend());
        return *this;
    }

    inline const std::vector<std::shared_ptr<DMX::Fixture>> &operator()()
    {
        return m_fixtures;
    }
};
