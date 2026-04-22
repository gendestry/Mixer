//
// Created by bobi on 19. 02. 26.
//

#include "Effect.h"

namespace Effect {
    Effect::Effect(DMX::FixtureGroup &group, Type type) {
        m_parameters = group.getParameters(DMX::Parameters::Type::COLOR);
        m_type = type;
    }

    void Effect::setEffect(EffectFunction eff) {
        m_effectFunction = std::move(eff);
    }

    void Effect::setBPM(uint16_t bpm) {
        this->bpm = bpm;
    }

    void Effect::update(float t) {
        // auto bps = bpm / 60.f;
        // float et = engineTick++ * bps;
        // if (et >= 60.f)
        // {
        m_effectFunction(tick++);
        // engineTick = engineTick % 60;
        // }
        // else {
        // m_effectFunction(tick);
        // }
    }
}
