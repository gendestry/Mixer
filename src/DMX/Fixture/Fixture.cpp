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
        m_totalSize = other.m_totalSize;
        start = other.start;
        // m_FID = other.m_FID;
        m_name = other.m_name;
        m_bytes = other.m_bytes;

        m_Parameters.reserve(other.m_Parameters.size());

        uint16_t offset = start;
        for (int i = 0; i < other.m_Parameters.size(); i++)
        {
            m_Parameters.push_back(std::make_shared<Parameters::Parameter>(*other.m_Parameters[i]));
            m_indexes[m_Parameters[i]->getType()].push_back(m_Parameters.back());
            // std::cout << "Offset: " << offset << std::endl;
            m_Parameters.back()->setOffset(offset);
            // std::cout
            m_Parameters.back()->setBuffer(m_bytes);
            // std::cout << m_indexes[m_Parameters[i]->getType()].back()->describe() << std::endl;
            // std::cout <<m_Parameters[i]->describe() << std::endl;

            offset += m_Parameters[i]->getSize();
        }

        // for (auto [k, v] : m_)
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