#pragma once

#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

#include "Effects/Effect.h"

//
// EffectEngine: the sole owner of effects. update() runs every enabled effect
// in insertion order each frame.
//
namespace Core::Effects
{
    class EffectEngine
    {
        std::vector<std::unique_ptr<Effect>> m_effects;

    public:
        Effect* add(std::unique_ptr<Effect> fx);

        // Construct an effect in place and return a non-owning pointer to it.
        template<typename T, typename... Args>
        T* emplace(Args&&... args)
        {
            auto fx = std::make_unique<T>(std::forward<Args>(args)...);
            T* raw = fx.get();
            m_effects.push_back(std::move(fx));
            return raw;
        }

        void remove(Effect* fx);
        void clear();
        void update(const TimeContext& t);

        [[nodiscard]] std::size_t size() const { return m_effects.size(); }
    };
}
