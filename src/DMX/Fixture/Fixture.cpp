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
        : Fragment(name)
    {}

    Fixture::Fixture(const Fixture& other)
    {
        size = other.size;
        start = other.start;
        // m_FID = other.m_FID;
        name = other.name;
        buffer = other.buffer;

        for (auto param : other.m_Parameters)
        {
            m_Parameters.push_back(std::make_shared<Parameters::Parameter>(*param));
            m_Parameters.back()->setOffset(start);
            m_Parameters.back()->setBuffer(buffer);
            m_indexes[m_Parameters.back()->getType()].push_back(m_Parameters.back());
        }
    }

    void Fixture::setStart(uint32_t st) {
        start = st;
        auto temp = start;
        for (auto param : m_Parameters)
        {
            param->setOffset(temp);
            temp += param->getSize();
        }
    }

    void Fixture::setBuffer(uint8_t* buf) {
        buffer = buf;
        for (auto param : m_Parameters)
        {
            param->setBuffer(buffer);
        }
    }

    std::optional<std::list<std::shared_ptr<Parameters::Parameter>>> Fixture::getParameters(Parameters::ParameterTypes type)
    {
        if (m_indexes.contains(type))
        {
            return m_indexes[type];
        }
        return std::nullopt;
    }

    std::optional<std::list<std::shared_ptr<Parameters::Parameter>>> Fixture::operator[](Parameters::ParameterTypes type)
    {
        return getParameters(type);
    }

    // std::string Fixture::getName() const
    // {
    //     return m_name;
    // }
    //
    // std::size_t Fixture::getTotalSize() const
    // {
    //     return size;
    // }

    std::string Fixture::describe() const
    {
        std::stringstream ss;
        ss << "Start: "<< start << std::endl;
        printf("%p\n", buffer);
        for (auto& p : m_Parameters)
        {
            ss <<  p->describe();
        }

        return ss.str();
    }

    uint8_t* Fixture::getBytes() const { return buffer; }
}