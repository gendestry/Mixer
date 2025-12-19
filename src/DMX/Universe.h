//
// Created by bobi on 15/09/2025.
//

#pragma once
#include <cstdint>
#include <list>
#include <unordered_map>

#include "DMX/Fixture/Fixture.h"
#include "Helper/FragmentedStorage.h"
// #include "Helper/SacnSender.h"

namespace DMX
{
    struct Interval
    {
        std::string name;
        uint16_t start;
        uint16_t end;

        Interval(std::string name, int start, int end) : name(name), start(start), end(end) {}
    };

    class Universe : public Utils::FragmentedStorage<Fixture, 512>, public Traits::Printable
    {
        uint16_t m_universeID;
        std::unordered_map<std::string, std::list<std::shared_ptr<Fixture>>> m_fixturesByName;


    public:
        Universe() : m_universeID(0){}
        explicit Universe(uint16_t universe)
            : m_universeID(universe)
        {
        }

        void setID(uint16_t universe)
        {
            m_universeID = universe;
        }

        void addFixture(std::shared_ptr<Fixture> fix, std::optional<uint32_t> start = std::nullopt);

        [[nodiscard]] uint16_t getUniverseID() const { return m_universeID; }
        [[nodiscard]] const std::shared_ptr<Fixture>& getLight(std::size_t index);
        // [[nodiscard]] std::vector<std::shared_ptr<Fixture>> getLights(std::string name);

        // [[nodiscard]] Fixture& operator[](int index);
        [[nodiscard]] std::list<std::shared_ptr<Fixture>> operator[](std::string name) { return m_fixturesByName[name]; }
        // [[nodiscard]] std::vector<std::shared_ptr<Fixture>> operator[](Interval interval);

        // [[nodiscard]] std::vector<uint8_t> operator()() const { return getBytes(); }

        [[nodiscard]] std::string describe() const override;


    };
}
