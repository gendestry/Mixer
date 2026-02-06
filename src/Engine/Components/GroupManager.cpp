//
// Created by bobi on 27. 01. 26.
//

#include "Engine/Components/GroupManager.h"
#include <sstream>

namespace Components
{

    void GroupManager::addToGroup(const std::string &name, std::shared_ptr<DMX::Fixture> fixture)
    {
        m_groups[name] += fixture;
    }

    void GroupManager::addToGroup(const std::string &name, const std::list<std::shared_ptr<DMX::Fixture>>& fixtures)
    {
        for (auto fixture : fixtures)
        {
            m_groups[name] += fixture;
        }
    }


    void GroupManager::addToGroup(const std::string &name, const std::string &otherName)
    {
        m_groups[name] += m_groups[otherName];
    }

    DMX::FixtureGroup &GroupManager::group(const std::string &name)
    {
        return m_groups[name];
    }

    std::string GroupManager::describe() const
    {
        std::stringstream ss;
        for (const auto&[key, value] : m_groups)
        {
            ss << "Fixture group: " << key << std::endl;
            const auto &fixs = value.get();
            for (auto fix : fixs)
            {
                ss << " - " << fix->describe() << std::endl;
            }
            ss << std::endl;
        }

        return ss.str();
    }

}