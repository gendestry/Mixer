#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

#include "DMX/Universe.h"
#include "Fixture/Fixture.h"
#include "Engine/Components/FixtureLibrary.h"

//
// Patch: owns the universes and the mapping from Fixture ID (FID) to the live
// fixture instances placed in them. Patching a fixture means copying a library
// definition into a universe (which wires it to the DMX buffer) and assigning
// it one or more FIDs. Patch is the single source of truth for which universes
// have changed (dirty set) so the output stage knows what to send.
//
namespace Core::Engine::Components
{
    class Patch
    {
        using FixturePtr = std::shared_ptr<Core::Fixture>;

        FixtureLibrary                            m_library;
        std::map<uint16_t, Core::DMX::Universe>   m_universes;       // by universe id
        std::map<uint16_t, FixturePtr>            m_fixtures;        // by FID
        std::unordered_map<std::string, std::vector<FixturePtr>> m_byName;
        std::set<uint16_t>                        m_usedFids;
        std::set<uint16_t>                        m_dirty;           // universes needing output

        Core::DMX::Universe& ensureUniverse(uint16_t universe);
        [[nodiscard]] uint16_t nextFreeFid() const;
        void registerFixture(uint16_t fid, const FixturePtr& fixture);

    public:
        [[nodiscard]] FixtureLibrary& library() { return m_library; }

        // Patch `amount` copies of a definition into `universe`. If `start` is
        // given the copies are packed contiguously from that channel, otherwise
        // each is auto-placed in the first free gap. FIDs are sequential from
        // `startFID` (skipping used ones) or auto-assigned. Returns the FIDs.
        std::vector<uint16_t> patch(const std::string& fixtureName, uint16_t universe, uint16_t amount,
                                    std::optional<uint32_t> start = std::nullopt,
                                    std::optional<uint16_t> startFID = std::nullopt);

        std::vector<uint16_t> patch(const Core::Fixture& fixture, uint16_t universe, uint16_t amount,
                                    std::optional<uint32_t> start = std::nullopt,
                                    std::optional<uint16_t> startFID = std::nullopt);

        // ---- lookup ----
        [[nodiscard]] Core::DMX::Universe* getUniverse(uint16_t universe);
        [[nodiscard]] const std::map<uint16_t, Core::DMX::Universe>& universes() const { return m_universes; }
        [[nodiscard]] FixturePtr getFixture(uint16_t fid) const;
        [[nodiscard]] std::vector<FixturePtr> getFixtures(const std::vector<uint16_t>& fids) const;
        [[nodiscard]] const std::vector<FixturePtr>& getFixturesByName(const std::string& name) const;

        // ---- dirty tracking ----
        [[nodiscard]] const std::set<uint16_t>& dirtyUniverses() const { return m_dirty; }
        void markDirty(uint16_t universe) { m_dirty.insert(universe); }
        void clearDirty() { m_dirty.clear(); }

        [[nodiscard]] std::string describe() const;
    };
}
