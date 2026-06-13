#include "Engine/Engine.h"

namespace Core::Engine
{
    void Engine::setIP(const std::string& ip)              { m_output.setIP(ip); }
    void Engine::setIP(const Utils::Network::IP& ip)       { m_output.setIP(ip); }
    void Engine::setSourceName(const std::string& name)    { m_output.setSourceName(name); }

    Components::FixtureLibrary& Engine::library()          { return m_patch.library(); }
    Core::Fixture& Engine::define(const std::string& name) { return m_patch.library().define(name); }

    std::vector<uint16_t> Engine::patch(const std::string& fixtureName, uint16_t universe, uint16_t amount,
                                        std::optional<uint32_t> start, std::optional<uint16_t> startFID)
    {
        return m_patch.patch(fixtureName, universe, amount, start, startFID);
    }

    DMX::FixtureGroup& Engine::group(const std::string& name)
    {
        auto [it, _] = m_groups.try_emplace(name, name);
        return it->second;
    }

    DMX::FixtureGroup& Engine::addToGroup(const std::string& name, const std::vector<uint16_t>& fids)
    {
        DMX::FixtureGroup& g = group(name);
        g.add(m_patch.getFixtures(fids));
        return g;
    }

    DMX::FixtureGroup* Engine::getGroup(const std::string& name)
    {
        const auto it = m_groups.find(name);
        return it != m_groups.end() ? &it->second : nullptr;
    }

    std::shared_ptr<Core::Fixture> Engine::getFixture(uint16_t fid)
    {
        return m_patch.getFixture(fid);
    }

    DMX::Universe* Engine::getUniverse(uint16_t universe)
    {
        return m_patch.getUniverse(universe);
    }

    Show::Sequence& Engine::sequence(const std::string& name)
    {
        auto [it, _] = m_sequences.try_emplace(name, name);
        return it->second;
    }

    void Engine::storeCue(const std::string& sequenceName, float number)
    {
        sequence(sequenceName).store(m_programmer.makeCue(number));
    }

    void Engine::go(const std::string& sequenceName)
    {
        sequence(sequenceName).go(m_patch);
    }

    void Engine::update()
    {
        // Advance the clock and build this frame's timing context.
        const unsigned int now = m_clock.stop();   // ms since the engine started
        Effects::TimeContext ctx;
        ctx.elapsed = now / 1000.0;
        ctx.dt      = (now - m_prevMs) / 1000.0;
        ctx.tick    = m_tick++;
        m_prevMs    = now;

        // Clear last frame's output so anything not addressed this frame goes
        // dark (non-tracking: each cue/look is a complete state).
        m_patch.blackout();

        // Playback layer first: every active sequence renders its current cue.
        for (auto& [name, seq] : m_sequences)
            seq.apply(ctx, m_patch);

        // Then the programmer on top (live edits win over playback).
        m_programmer.apply(ctx);

        // Compose virtual dimmers once, after every layer has written colour
        // and intensity (no-op for fixtures with a real dimmer channel).
        for (const auto& [fid, fixture] : m_patch.fixtures())
            fixture->applyVirtualDimmers();

        // Continuous full-frame output, as a real sACN source does.
        m_output.sendAll(m_patch);
        m_patch.clearDirty();
    }

    std::string Engine::describe() const
    {
        std::string s = m_patch.describe();
        s += "\nGroups [" + std::to_string(m_groups.size()) + "]";
        for (const auto& [name, g] : m_groups)
            s += "\n  - " + g.describe();
        return s;
    }
}
