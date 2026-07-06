#pragma once

#include <string>
#include <utility>

//
// Attribute: the base for every named, poolable console object (Groups and
// Presets today; Palettes/Views next). It carries the shared identity - a name
// and an integer id (the pool slot) - and a polymorphic describe(). Payloads
// live in the subclasses; everything an Attribute stores is FID-based and
// serializable (never a live shared_ptr<Fixture>).
//
namespace LightEngine::Attributes
{
    class Attribute
    {
        std::string m_name;
        int         m_id = 0;

    public:
        Attribute() = default;
        explicit Attribute(std::string name) : m_name(std::move(name)) {}
        virtual ~Attribute() = default;

        [[nodiscard]] const std::string& name() const { return m_name; }
        void setName(std::string name) { m_name = std::move(name); }

        [[nodiscard]] int id() const { return m_id; }
        void setId(int id) { m_id = id; }

        [[nodiscard]] virtual std::string describe() const = 0;
    };
}
