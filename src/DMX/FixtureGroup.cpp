//
// Created by Bobi on 9/22/25.
//

#include "FixtureGroup.h"

namespace DMX
{
    void FixtureGroup::addParameters(const std::shared_ptr<Fixture>& fixture)
    {
        using namespace Parameters;

        auto colorParams = fixture->getParameters(Type::COLOR);
        if (colorParams.has_value())
        {
            for (auto& param : colorParams.value())
            {
                m_parameters[Type::COLOR].push_back(param);
            }
        }

        auto dimmerParams = fixture->getParameters(Type::DIMMER);
        if (dimmerParams.has_value())
        {
            for (auto& param : dimmerParams.value())
            {
                m_parameters[Type::DIMMER].push_back(param);
            }
        }

        auto positionParams = fixture->getParameters(Type::POSITION);
        if (positionParams.has_value())
        {
            for (auto& param : positionParams.value())
            {
                m_parameters[Type::POSITION].push_back(param);
            }
        }
    }

    void FixtureGroup::addParameters(const std::list<std::shared_ptr<Fixture>>& fixtures)
    {
        for (auto& fixture : fixtures)
        {
            addParameters(fixture);
        }
    }

    void FixtureGroup::addParameters(const std::vector<std::shared_ptr<Fixture>>& fixtures)
    {
        for (auto& fixture : fixtures)
        {
            addParameters(fixture);
        }
    }

    void FixtureGroup::clearParameters()
    {
        m_parameters.clear();
    }


    void FixtureGroup::add(const std::shared_ptr<DMX::Fixture>& fixture)
    {
        m_usedUniverses.insert(fixture->m_universe);
        m_fixtures.push_back(fixture);
        addParameters(fixture);
    }

    void FixtureGroup::add(const std::list<std::shared_ptr<DMX::Fixture>>& fixs)
    {
        m_fixtures.insert(m_fixtures.end(), fixs.begin(), fixs.end());
        for(auto fix : fixs)
        {
            m_usedUniverses.insert(fix->m_universe);
        }
        addParameters(fixs);
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
        // clearParameters();
        // add
        return *this;
    }

    FixtureGroup& FixtureGroup::operator+=(const std::shared_ptr<DMX::Fixture>& fixture)
    {
        this->m_fixtures.push_back(fixture);
        this->m_usedUniverses.insert(fixture->m_universe);
        addParameters(fixture);
        return *this;
    }

    FixtureGroup& FixtureGroup::operator+=(const std::list<std::shared_ptr<DMX::Fixture>> &fixtures)
    {
        this->m_fixtures.insert(this->m_fixtures.end(), fixtures.begin(), fixtures.end());
        for(auto fix : fixtures)
        {
            this->m_usedUniverses.insert(fix->m_universe);
        }
        addParameters(fixtures);
        return *this;
    }

    FixtureGroup& FixtureGroup::operator+=(FixtureGroup &other)
    {
        auto otherLights = other.m_fixtures;
        this->m_fixtures.insert(this->m_fixtures.end(), otherLights.begin(), otherLights.end());
        for(auto fix : otherLights)
        {
            this->m_usedUniverses.insert(fix->m_universe);
        }
        addParameters(other.m_fixtures);
        return *this;
    }

    // FixtureGroup& FixtureGroup::operator-=(const std::list<std::shared_ptr<DMX::Fixture>> &lights)
    // {
    //     this->m_fixtures.insert(this->m_fixtures.end(), lights.rbegin(), lights.rend());
    //     return *this;
    // }
    //
    // FixtureGroup& FixtureGroup::operator-=(FixtureGroup &other)
    // {
    //     auto otherLights = other();
    //     this->m_fixtures.insert(this->m_fixtures.end(), otherLights.rbegin(), otherLights.rend());
    //     return *this;
    // }

    const std::vector<std::shared_ptr<DMX::Fixture>>& FixtureGroup::operator()()
    {
        return m_fixtures;
    }

}
