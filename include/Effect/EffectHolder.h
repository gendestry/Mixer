//
// Created by Bobi on 9/22/25.
//

#pragma once
#include "DMX/FixtureGroup.h"

struct Effect {

};

struct IntensityEffect : public Effect {};
struct ColorEffect : public Effect {};

class EffectHolder
{
    std::vector<DMX::FixtureGroup> m_groups;
public:
    void addGroup(const DMX::FixtureGroup& group)
    {
        m_groups.push_back(group);
    }

    // template <typename T>
    // requires std::is_base_of_v<Effect, T>
    // void addEffect()
};
