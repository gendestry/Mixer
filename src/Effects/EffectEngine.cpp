#include "LightEngine/Effects/EffectEngine.h"

namespace LightEngine::Effects
{
    Effect* EffectEngine::add(std::unique_ptr<Effect> fx)
    {
        Effect* raw = fx.get();
        m_effects.push_back(std::move(fx));
        return raw;
    }

    void EffectEngine::remove(Effect* fx)
    {
        std::erase_if(m_effects, [&](const std::unique_ptr<Effect>& e) { return e.get() == fx; });
    }

    void EffectEngine::clear()
    {
        m_effects.clear();
    }

    void EffectEngine::update(const TimeContext& t)
    {
        for (const auto& fx : m_effects)
            if (fx->enabled())
                fx->apply(t);
    }
}
