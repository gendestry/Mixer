//
// Created by Bobi on 9/22/25.
//

#pragma once
#include <memory>
#include <vector>
#include <list>

#include "DMX/Fixture/Fixture.h"

namespace DMX
{
    class FixtureGroup
    {
        std::vector<std::shared_ptr<Fixture>> m_fixtures;
        std::string name;
        // uint16_t m_id;

        std::unordered_map<Parameters::ParameterTypes, std::vector<std::shared_ptr<Parameters::Parameter>>> m_parameters;

        void addParameters(const std::shared_ptr<Fixture>& fixtures);
        void addParameters(const std::list<std::shared_ptr<Fixture>>& fixtures);
        void addParameters(const std::vector<std::shared_ptr<Fixture>>& fixtures);
        void clearParameters();

    public:
        FixtureGroup() = default;
        explicit FixtureGroup(const std::string& name) : name(name) {};

        void add(const std::shared_ptr<Fixture>& fixture);

        void add(const std::list<std::shared_ptr<Fixture>>& fixs);
        const std::vector<std::shared_ptr<Fixture>>& get();

        [[nodiscard]] size_t size() const;

        FixtureGroup &operator=(const std::vector<std::shared_ptr<Fixture>> &fixtures);

        FixtureGroup &operator+=(const std::shared_ptr<Fixture>& fixture);
        FixtureGroup &operator+=(const std::list<std::shared_ptr<Fixture>> &fixtures);
        FixtureGroup &operator+=(FixtureGroup &other);

        // FixtureGroup &operator-=(const std::list<std::shared_ptr<Fixture>> &lights);
        // FixtureGroup &operator-=(FixtureGroup &other);

        const std::vector<std::shared_ptr<Fixture>> &operator()();
        std::vector<std::shared_ptr<Parameters::Parameter>>& getParameters(Parameters::ParameterTypes ptype)
        {
            return m_parameters[ptype];
        }
    };

}
