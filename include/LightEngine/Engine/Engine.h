#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "Utils/Network/IP.h"
#include "Utils/Time/Timer.h"

#include "LightEngine/Attributes/Feature.h"
#include "LightEngine/Attributes/FxPreset.h"
#include "LightEngine/Attributes/Group.h"
#include "LightEngine/Attributes/Pool.h"
#include "LightEngine/Attributes/Preset.h"
#include "LightEngine/DMX/FixtureGroup.h"
#include "LightEngine/Engine/Components/DMXOutput.h"
#include "LightEngine/Engine/Components/Patch.h"
#include "LightEngine/Engine/Programmer.h"
#include "LightEngine/Show/Sequence.h"

//
// Engine: the top-level orchestrator. It owns the Patch (universes + fixtures),
// the DMXOutput (sACN senders) and the named fixture groups, and exposes a
// small high-level API: define -> patch -> group -> drive -> update. update()
// pushes the current frame of every universe to the network.
//
namespace LightEngine::Engine
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

        // ---- pools (poolable Attributes, keyed by cell id) ----
        Attributes::Pool<Attributes::Group>    m_groupPool;
        Attributes::Pool<Attributes::Preset>   m_colorPool;
        Attributes::Pool<Attributes::Preset>   m_intensityPool;
        Attributes::Pool<Attributes::Preset>   m_positionPool;
        Attributes::Pool<Attributes::Preset>   m_dimmerPool;
        Attributes::Pool<Attributes::FxPreset> m_fxPool;

        Attributes::Pool<Attributes::Preset>& presetPool(Attributes::Feature feature);

    public:
        Engine() = default;

        // ---- output ----
        void setIP(const std::string& ip);
        void setIP(const Utils::Network::IP& ip);
        void setSourceName(const std::string& name);

        // ---- definitions & patching ----
        Components::FixtureLibrary& library();
        LightEngine::Fixture& define(const std::string& name);
        std::vector<uint16_t> patch(const std::string& fixtureName, uint16_t universe, uint16_t amount,
                                    std::optional<uint32_t> start = std::nullopt,
                                    std::optional<uint16_t> startFID = std::nullopt);

        // ---- groups ----
        DMX::FixtureGroup& group(const std::string& name);   // get or create
        DMX::FixtureGroup& addToGroup(const std::string& name, const std::vector<uint16_t>& fids);
        [[nodiscard]] DMX::FixtureGroup* getGroup(const std::string& name);

        // ---- programmer (live editing layer) ----
        [[nodiscard]] Programmer& programmer() { return m_programmer; }

        // ---- pools: store (programmer -> pool) ----
        void storeGroup(int id, const std::string& name = "");                       // selection -> Group pool
        void storePreset(Attributes::Feature feature, int id, const std::string& name = "");  // values -> feature pool
        void storeFx(int id, const std::string& name = "");                          // effects -> FX pool

        // ---- pools: recall (pool -> programmer) ----
        void selectGroup(int id);                        // resolve + add group to selection
        void applyPreset(Attributes::Feature feature, int id);   // stamp preset onto selection

        // ---- pool accessors (for UI / inspection) ----
        [[nodiscard]] Attributes::Pool<Attributes::Group>&    groupPool() { return m_groupPool; }
        [[nodiscard]] Attributes::Pool<Attributes::Preset>&   colorPool() { return m_colorPool; }
        [[nodiscard]] Attributes::Pool<Attributes::FxPreset>& fxPool()    { return m_fxPool; }

        // ---- sequences / cues ----
        Show::Sequence& sequence(const std::string& name);          // get or create
        void storeCue(const std::string& sequence, float number);   // programmer -> cue
        void go(const std::string& sequence);                       // advance a sequence

        // ---- lookup ----
        [[nodiscard]] std::shared_ptr<LightEngine::Fixture> getFixture(uint16_t fid);
        [[nodiscard]] DMX::Universe* getUniverse(uint16_t universe);
        [[nodiscard]] Components::Patch& patcher() { return m_patch; }

        // ---- render ----
        void update();   // send the current frame of every universe over sACN

        [[nodiscard]] std::string describe() const;
    };
}
