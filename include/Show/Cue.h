#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "Effects/EffectSpec.h"
#include "Fixture/Values.h"

//
// Cue: a stored, recallable look. Values are per-fixture (by FID) so the cue is
// stable and serializable; effects are stored as Specs (recipes), not live
// objects. Recall instantiates the effects via the EffectFactory.
//
namespace Core::Show
{
    struct Cue
    {
        float       number  = 1.0f;
        std::string name;
        float       fadeIn  = 0.0f;
        float       fadeOut = 0.0f;
        float       delay   = 0.0f;

        std::map<uint16_t, Core::Values> values;    // FID -> attribute values
        std::vector<Effects::Spec>       effects;   // effect recipes
    };
}
