//
// Created by bobi on 17/09/2025.
//

#include "DMX/Fixture/Fixture.h"
#include <sstream>
#include <iostream>
#include <format>

namespace DMX
{

    Fixture::Fixture(const std::string& name)
        : m_name(name)
    {}

    Fixture::Fixture(const Fixture& other)
    {
        m_Parameters = other.m_Parameters;
        m_indexes = other.m_indexes;
        m_totalSize = other.m_totalSize;
        start = other.start;
        // m_FID = other.m_FID;
        m_name = other.m_name;
        m_bytes = other.m_bytes;

        for (auto& parameter : m_Parameters)
        {
            parameter.get()->setBuffer(m_bytes);
        }
    }

    std::optional<std::reference_wrapper<std::vector<std::shared_ptr<Parameters::Parameter>>>> Fixture::getParameters(Parameters::ParameterTypes type)
    {
        if (m_indexes.contains(type))
        {
            return m_indexes[type];
        }
        return std::nullopt;
    }

    std::optional<std::reference_wrapper<std::vector<std::shared_ptr<Parameters::Parameter>>>> Fixture::operator[](Parameters::ParameterTypes type)
    {
        return getParameters(type);
    }

    std::string Fixture::getName() const
    {
        return m_name;
    }

    std::size_t Fixture::getTotalSize() const
    {
        return m_totalSize;
    }

    std::string Fixture::describe() const
    {
        std::stringstream ss;
        ss << "Start: "<< start << std::endl;
        for (auto& p : m_Parameters)
        {
            ss <<  p->describe();
        }

        return ss.str();
    }

    uint8_t* Fixture::getBytes() const { return m_bytes; }
}