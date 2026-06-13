#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "Utils/Network/IP.h"
#include "Utils/Time/Timer.h"

#include "DMX/FixtureGroup.h"
#include "Engine/Components/DMXOutput.h"
#include "Engine/Components/Patch.h"
#include "Engine/Programmer.h"
#include "Show/Sequence.h"

//
// Engine: the top-level orchestrator. It owns the Patch (universes + fixtures),
// the DMXOutput (sACN senders) and the named fixture groups, and exposes a
// small high-level API: define -> patch -> group -> drive -> update. update()
// pushes the current frame of every universe to the network.
//
namespace Core::Engine
{
    class Engine
    {
        Components::Patch                        m_patch;
        Components::DMXOutput                    m_output;
        std::map<std::string, DMX::FixtureGroup> m_groups;
        std::map<std::string, Show::Sequence>    m_sequences;
        Programmer                               m_programmer;
        Utils::Time::Timer                       m_clock;       // started at construction
        unsigned int                             m_prevMs = 0;
        uint32_t                                 m_tick   = 0;

    public:
        Engine() = default;

        // ---- output ----
        void setIP(const std::string& ip);
        void setIP(const Utils::Network::IP& ip);
        void setSourceName(const std::string& name);

        // ---- definitions & patching ----
        Components::FixtureLibrary& library();
        Core::Fixture& define(const std::string& name);
        std::vector<uint16_t> patch(const std::string& fixtureName, uint16_t universe, uint16_t amount,
                                    std::optional<uint32_t> start = std::nullopt,
                                    std::optional<uint16_t> startFID = std::nullopt);

        // ---- groups ----
        DMX::FixtureGroup& group(const std::string& name);   // get or create
        DMX::FixtureGroup& addToGroup(const std::string& name, const std::vector<uint16_t>& fids);
        [[nodiscard]] DMX::FixtureGroup* getGroup(const std::string& name);

        // ---- programmer (live editing layer) ----
        [[nodiscard]] Programmer& programmer() { return m_programmer; }

        // ---- sequences / cues ----
        Show::Sequence& sequence(const std::string& name);          // get or create
        void storeCue(const std::string& sequence, float number);   // programmer -> cue
        void go(const std::string& sequence);                       // advance a sequence

        // ---- lookup ----
        [[nodiscard]] std::shared_ptr<Core::Fixture> getFixture(uint16_t fid);
        [[nodiscard]] DMX::Universe* getUniverse(uint16_t universe);
        [[nodiscard]] Components::Patch& patcher() { return m_patch; }

        // ---- render ----
        void update();   // send the current frame of every universe over sACN

        [[nodiscard]] std::string describe() const;
    };
}
