//
// Created by Bobi on 9/22/25.
//

#include "DMX/FixtureGroup.h"

namespace DMX
{
    void FixtureGroup::add(const std::shared_ptr<DMX::Fixture>& fixture)
    {
        m_fixtures.push_back(fixture);
    }

    void FixtureGroup::add(const std::list<std::shared_ptr<DMX::Fixture>>& fixs)
    {
        m_fixtures.insert(m_fixtures.end(), fixs.begin(), fixs.end());
    }

    const std::vector<std::shared_ptr<DMX::Fixture>>& FixtureGroup::get()
    {
        return m_fixtures;
    }

    size_t FixtureGroup::size() const
    {
        return m_fixtures.size();
    }


    FixtureGroup& FixtureGroup::operator=(const std::vector<std::shared_ptr<DMX::Fixture>> &fixtures)
    {
        this->m_fixtures = fixtures;
        return *this;
    }

    FixtureGroup& FixtureGroup::operator+=(const std::shared_ptr<DMX::Fixture>& fixture)
    {
        this->m_fixtures.push_back(fixture);
        return *this;
    }

    FixtureGroup& FixtureGroup::operator+=(const std::list<std::shared_ptr<DMX::Fixture>> &fixtures)
    {
        this->m_fixtures.insert(this->m_fixtures.end(), fixtures.begin(), fixtures.end());
        return *this;
    }

    FixtureGroup& FixtureGroup::operator+=(FixtureGroup &other)
    {
        auto otherLights = other();
        this->m_fixtures.insert(this->m_fixtures.end(), otherLights.begin(), otherLights.end());
        return *this;
    }

    FixtureGroup& FixtureGroup::operator-=(const std::list<std::shared_ptr<DMX::Fixture>> &lights)
    {
        this->m_fixtures.insert(this->m_fixtures.end(), lights.rbegin(), lights.rend());
        return *this;
    }

    FixtureGroup& FixtureGroup::operator-=(FixtureGroup &other)
    {
        auto otherLights = other();
        this->m_fixtures.insert(this->m_fixtures.end(), otherLights.rbegin(), otherLights.rend());
        return *this;
    }

    const std::vector<std::shared_ptr<DMX::Fixture>>& FixtureGroup::operator()()
    {
        return m_fixtures;
    }

}
