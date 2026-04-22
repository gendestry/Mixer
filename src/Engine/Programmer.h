// #pragma once
// #include "DMX/FixtureGroup.h"
// #include "Effect/Effect.h"
// #include "../Helper/Curve.h"
// #include "DMX/Fixture/Parameters/Parameters.h"
//
// class Programmer
// {
// protected:
//     DMX::FixtureGroup* selectedGroup;
//     Effect::Effect* currentEffect;
//
// public:
//     void setGroup(DMX::FixtureGroup* other)
//     {
//         selectedGroup = other;
//     }
//
//     template <typename TEffect>
//     void addEffect(std::function<void()> eff)
//     {
//         currentEffect = new TEffect(selectedGroup);
//         currentEffect->setEffect(eff);
//         // selectedGroup->getParameters(type);
//     };
//
//     void setCurveType(Utils::Curve::Type type)
//     {
//         currentEffect->setCurveType(type);
//     }
//
//     void setCurvePeaks(uint16_t peaks)
//     {
//         currentEffect->setCurvePeaks(peaks);
//     }
//
//     void setCurveLength(std::size_t length)
//     {
//         currentEffect->setCurveLength(length);
//     }
//
//
//
//     void appendToGroup();
//
//     void update();
//     void clear()
//     {
//
//     }
// };
#pragma once
#include "DMX/FixtureGroup.h"
#include "Effect/Effect.h"
#include "../Helper/Curve.h"
#include "DMX/Fixture/Parameters/Parameters.h"
#include <list>
#include <memory>

struct State {
    DMX::FixtureGroup group;
    std::list<std::shared_ptr<Effect::Effect> > effects;
};

class Programmer {
protected:
    std::optional<DMX::FixtureGroup> selectedGroups;
    std::list<std::shared_ptr<Effect::Effect> > selectedEffects;

    std::list<State> states;

public:
    void selectGroup(DMX::FixtureGroup &other) {
        if (!selectedGroups.has_value()) {
            selectedGroups = other;
            // current.group = other;
        } else {
            if (!selectedEffects.empty()) {
                State state;
                state.group = selectedGroups.value();
                state.effects = std::move(selectedEffects);
                states.push_back(std::move(state));
            }
            selectedGroups.value() += other;
        }
    }

    void newGroupSelection() {
        State state;
        state.group = selectedGroups.value();
        state.effects = std::move(selectedEffects);
        states.push_back(std::move(state));
        selectedGroups.reset();
    }

    void clear() {
        selectedGroups.reset();
        selectedEffects.clear();
        states.clear();
    }

    void setDimmer(float perc) {
        selectedEffects.push_back(
            std::make_shared<Effect::DimmerEffect>(selectedGroups.value(), perc));
    }

    void setColor(Utils::Colors::RGB color) {
        selectedEffects.push_back(
            std::make_shared<Effect::ColorEffect>(selectedGroups.value(), color));
    }

    void setColorFan(Utils::Colors::RGB color, Utils::Colors::RGB color2) {
        selectedEffects.push_back(
            std::make_shared<Effect::ColorFanEffect>(selectedGroups.value(), color, color2));
    }

    void setColorGradient(const std::vector<Utils::Colors::RGB> &colors, const std::vector<float> &percentages) {
        selectedEffects.push_back(
            std::make_shared<Effect::ColorGradientEffect>(selectedGroups.value(), colors, percentages));
    }

    std::shared_ptr<Effect::FXDimmerChase> addDimmerChase() {
        auto ret = std::make_shared<Effect::FXDimmerChase>(selectedGroups.value());
        selectedEffects.push_back(ret);
        return ret;
    }

    void update(float tick) {
        for (auto &[_, effects]: states) {
            for (const auto &eff: effects) {
                eff->update(tick);
            }
        }
        for (const auto &s: selectedEffects) {
            s->update(tick);
        }
    }

    std::list<State> record() {
        std::list<State> ret = std::move(states);

        if (selectedGroups.has_value() && !selectedEffects.empty()) {
            State state;
            state.group = selectedGroups.value();
            state.effects = std::move(selectedEffects);
            ret.push_back(std::move(state));
        }

        return ret;
    }
};
