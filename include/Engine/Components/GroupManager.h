//
// Created by bobi on 27. 01. 26.
//

#pragma once
#include <map>
#include "DMX/FixtureGroup.h"
#include <memory>
#include <list>

namespace Components
{
    class GroupManager
    {
        std::map<std::string, DMX::FixtureGroup> m_groups;
    public:
        void addToGroup(const std::string &name, std::shared_ptr<DMX::Fixture> fixture);
        void addToGroup(const std::string &name, const std::list<std::shared_ptr<DMX::Fixture>>& fixtures);
        void addToGroup(const std::string &name, const std::string &otherName);

        DMX::FixtureGroup& group(const std::string &name);

        // [[nodiscard]]GroupManager& operator+= (const std::string& name) { m_groups}
        [[nodiscard]] std::string describe() const;
        [[nodiscard]] DMX::FixtureGroup& operator[](const std::string& name) { return group(name); }
    };
}