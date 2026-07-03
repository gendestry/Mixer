#include "LightEngine/Fixture/Fixture.h"

#include <utility>

namespace LightEngine
{
    using Parameters::Type;
    using Parameters::Parameter;
    using Parameters::VDimmerParameter;

    Fixture::Fixture(std::string name) : m_name(std::move(name)) {}

    Fixture::Fixture(const Fixture& other)
        : Utils::Fragment(other), m_name(other.m_name), m_fid(other.m_fid), m_universe(other.m_universe)
    {
        buffer = nullptr;            // re-assigned when placed in a universe
        cloneParametersFrom(other);
    }

    Fixture& Fixture::operator=(const Fixture& other)
    {
        if (this == &other) return *this;
        size       = other.size;
        start      = other.start;
        id         = other.id;
        buffer     = nullptr;
        m_name     = other.m_name;
        m_fid      = other.m_fid;
        m_universe = other.m_universe;
        m_parameters.clear();
        m_byType.clear();
        cloneParametersFrom(other);
        return *this;
    }

    void Fixture::cloneParametersFrom(const Fixture& other)
    {
        m_parameters.reserve(other.m_parameters.size());
        for (const auto& p : other.m_parameters)
        {
            ParamPtr copy = p->clone();
            m_parameters.push_back(copy);
            m_byType[copy->getType()].push_back(copy);
        }
        rebindVirtualDimmers();
    }

    Parameter* Fixture::firstOfType(Type t) const
    {
        const auto it = m_byType.find(t);
        if (it != m_byType.end() && !it->second.empty())
            return it->second.front().get();
        return nullptr;
    }

    void Fixture::rebindVirtualDimmers()
    {
        Parameter* color = firstOfType(Type::COLOR);
        const auto it = m_byType.find(Type::VDIMMER);
        if (it == m_byType.end()) return;
        for (const auto& p : it->second)
            static_cast<VDimmerParameter*>(p.get())->bind(color);
    }

    Fixture::ParamPtr Fixture::add(const Parameters::Descriptor* desc)
    {
        if (desc == nullptr) return nullptr;

        ParamPtr p = std::make_shared<Parameter>(desc);
        p->setOffset(size);   // provisional local offset; made absolute in setStart()
        m_parameters.push_back(p);
        m_byType[desc->type].push_back(p);
        size += desc->size;   // grow the fixture footprint
        return p;
    }

    std::shared_ptr<VDimmerParameter> Fixture::addVirtualDimmer()
    {
        auto vd = std::make_shared<VDimmerParameter>(Parameters::Presets::Virtual());
        m_parameters.push_back(vd);
        m_byType[Type::VDIMMER].push_back(vd);
        vd->bind(firstOfType(Type::COLOR));   // zero footprint
        return vd;
    }

    void Fixture::ensureVirtualDimmer()
    {
        if (has(Type::DIMMER) || has(Type::VDIMMER)) return;
        if (!has(Type::COLOR)) return;
        addVirtualDimmer();
    }

    void Fixture::setStart(uint32_t st)
    {
        Utils::Fragment::setStart(st);
        uint32_t local = 0;
        for (const auto& p : m_parameters)
        {
            p->setOffset(st + local);
            local += p->getSize();
        }
    }

    void Fixture::setBuffer(uint8_t* buf)
    {
        Utils::Fragment::setBuffer(buf);
        for (const auto& p : m_parameters)
            p->setBuffer(buf);
    }

    bool Fixture::has(Type t) const
    {
        return m_byType.find(t) != m_byType.end();
    }

    const std::vector<Fixture::ParamPtr>& Fixture::parameters(Type t) const
    {
        static const std::vector<ParamPtr> empty;
        const auto it = m_byType.find(t);
        return it != m_byType.end() ? it->second : empty;
    }

    void Fixture::setColor(const Utils::Colors::RGB& color)
    {
        for (const auto& p : parameters(Type::COLOR))
            p->setColor(color);
    }

    void Fixture::setIntensity(float norm)
    {
        if (has(Type::DIMMER))
        {
            for (const auto& p : parameters(Type::DIMMER))
                p->setLevel(norm);
        }
        else
        {
            for (const auto& p : parameters(Type::VDIMMER))
                static_cast<VDimmerParameter*>(p.get())->setLevel(norm);
        }
    }

    void Fixture::applyVirtualDimmers()
    {
        for (const auto& p : parameters(Type::VDIMMER))
            static_cast<VDimmerParameter*>(p.get())->apply();
    }

    std::string Fixture::describe() const
    {
        return "FID " + std::to_string(m_fid) + " \"" + m_name + "\" [" + std::to_string(size)
             + " ch, " + std::to_string(m_parameters.size()) + " params]";
    }
}
