#include "Show/Sequence.h"

#include <algorithm>

#include "Effects/EffectFactory.h"
#include "Fixture/Fixture.h"

namespace Core::Show
{
    Cue& Sequence::store(const Cue& cue)
    {
        const auto it = std::find_if(m_cues.begin(), m_cues.end(),
                                     [&](const Cue& c) { return c.number == cue.number; });
        if (it != m_cues.end())
        {
            *it = cue;
            return *it;
        }
        // keep cues ordered by number
        const auto pos = std::find_if(m_cues.begin(), m_cues.end(),
                                      [&](const Cue& c) { return c.number > cue.number; });
        return *m_cues.insert(pos, cue);
    }

    void Sequence::instantiate(Core::Engine::Components::Patch& patch)
    {
        m_playback.clear();
        if (m_current < 0) return;

        for (const Effects::Spec& spec : m_cues[m_current].effects)
        {
            DMX::FixtureGroup group("cue");
            for (uint16_t fid : spec.fids)
                if (auto f = patch.getFixture(fid))
                    group.add(f);
            m_playback.add(Effects::EffectFactory::build(spec, std::move(group)));
        }
    }

    void Sequence::go(Core::Engine::Components::Patch& patch)
    {
        if (m_cues.empty()) return;
        m_current = (m_current + 1) % static_cast<int>(m_cues.size());
        instantiate(patch);
    }

    void Sequence::goTo(std::size_t index, Core::Engine::Components::Patch& patch)
    {
        if (index >= m_cues.size()) return;
        m_current = static_cast<int>(index);
        instantiate(patch);
    }

    void Sequence::apply(const Effects::TimeContext& t, Core::Engine::Components::Patch& patch)
    {
        if (m_current < 0) return;
        const Cue& cue = m_cues[m_current];
        const Utils::Colors::RGB white{255, 255, 255};

        // Default colour = white for every fixture the cue drives.
        for (const auto& [fid, v] : cue.values)
            if (auto f = patch.getFixture(fid)) f->setColor(white);
        for (const Effects::Spec& spec : cue.effects)
            for (uint16_t fid : spec.fids)
                if (auto f = patch.getFixture(fid)) f->setColor(white);

        // Static values.
        for (const auto& [fid, v] : cue.values)
        {
            auto f = patch.getFixture(fid);
            if (!f) continue;
            if (v.color)     f->setColor(*v.color);
            if (v.intensity) f->setIntensity(*v.intensity);
        }

        // Effects on top.
        m_playback.update(t);
    }

    std::string Sequence::describe() const
    {
        std::string s = "Sequence \"" + m_name + "\" [" + std::to_string(m_cues.size()) + " cues]";
        for (const Cue& c : m_cues)
            s += "\n  - Cue " + std::to_string(c.number) + " \"" + c.name + "\": "
               + std::to_string(c.values.size()) + " values, "
               + std::to_string(c.effects.size()) + " effects";
        return s;
    }
}
