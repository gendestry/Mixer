//
// Created by bobi on 15/09/2025.
//

#pragma once
#include <cstdint>
#include <list>
#include <unordered_map>

#include "Fixture/Fixture.h"
#include "Helper/FragmentedStorage.h"
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
        explicit Universe(uint16_t universe = 1)
            : m_universeID(universe)
        {
        }

        void addFixture(std::shared_ptr<Fixture> fix, std::optional<uint32_t> start = std::nullopt) {
            this->add(fix, start);
            m_fixturesByName[fix->name].push_back(m_fragments.back());
        }

        // void addMultiple(std::shared_ptr<Fixture> fragment, uint16_t amount, std::optional<uint16_t> start = std::nullopt)
        // {
        //     for (uint16_t i = 0; i < amount; i++)
        //     {
        //         addFixture(fragment, start);
        //     }
        // }

        // [[nodiscard]] uint16_t numLights() const { return this->m.size(); }
        // [[nodiscard]] uint16_t getUniverseID() const { return m_universeID; }

        [[nodiscard]] const std::shared_ptr<Fixture>& getLight(std::size_t index);
        // [[nodiscard]] std::vector<std::shared_ptr<Fixture>> getLights(std::string name);

        // [[nodiscard]] Fixture& operator[](int index);
        [[nodiscard]] std::list<std::shared_ptr<Fixture>> operator[](std::string name) { return m_fixturesByName[name]; }
        // [[nodiscard]] std::vector<std::shared_ptr<Fixture>> operator[](Interval interval);

        // [[nodiscard]] std::vector<uint8_t> operator()() const { return getBytes(); }

        [[nodiscard]] std::string describe() const override;


    };
}
