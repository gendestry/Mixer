//
// Created by bobi on 15/09/2025.
//

#pragma once
#include <array>
#include <cstdint>
#include <cstring>
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

    class Universe
    {
        static constexpr uint16_t MAX_SIZE = 512U;
        uint16_t m_universeID;
        FragmentedStorage<Fixture, MAX_SIZE> storage;
        // std::list<Fixture> m_fixtures;
        std::unordered_map<std::string, std::vector<std::shared_ptr<Fixture>>> m_fixturesByName;

        // std::array<uint8_t, MAX_SIZE> m_bytes;
        // uint8_t m_bytes[512];
        // uint16_t bytesPatched[MAX_SIZE] = {0};
        // std::

        std::string nextColor(int index) const;
    public:
        Universe(uint16_t universe = 1) : m_universeID(universe)
        {
            // memset(&m_bytes[0], 0U, 512);
        };

        // void fillBytesPatched(uint16_t start, uint16_t end);
        void add(Fixture &fragment, std::optional<uint16_t> start = std::nullopt)
        {
            storage.add(fragment, start);
        }
        // bool addMultiple(Fixture &fragment, uint16_t amount, std::optional<uint16_t> start = std::nullopt);

        // [[nodiscard]] uint16_t numLights() const { return this->m.size(); }
        // [[nodiscard]] uint16_t getUniverseID() const { return m_universeID; }

        // [[nodiscard]] std::shared_ptr<Fixture> getLight(std::size_t index);
        // [[nodiscard]] std::vector<std::shared_ptr<Fixture>> getLights(std::string name);

        // [[nodiscard]] Fixture& operator[](int index);
        // [[nodiscard]] std::vector<std::shared_ptr<Fixture>> operator[](std::string name) { return getLights(name); }
        // [[nodiscard]] std::vector<std::shared_ptr<Fixture>> operator[](Interval interval);

        // [[nodiscard]] std::vector<uint8_t> getBytes() const;
        // [[nodiscard]] std::vector<uint8_t> operator()() const { return getBytes(); }

        void printFragments();
        void printBytes() const;
        void print() const;

    };
}
