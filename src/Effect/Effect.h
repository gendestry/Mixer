#pragma once
#include <functional>
#include "DMX/FixtureGroup.h"
#include "DMX/Fixture/Parameters/Parameters.h"
#include "Curve.h"

namespace Effect
{

using EffectFunction = std::function<void()>;

class Effect 
{
protected:
    // DMX::FixtureGroup* m_group;
    std::vector<std::shared_ptr<DMX::Parameters::Parameter>> m_parameters; 
    EffectFunction m_effectFunction;
    Utils::Curve m_curve;

    // virtual void updateImpl() = 0;
public:
    struct Config
    {
        uint16_t bpm = 60;
    };

    Effect()
    // Effect(DMX::FixtureGroup& group, EffectFunction effectFunction)
        // : m_parameters(parameters), m_effectFunction(effectFunction)
    {}

    Effect(DMX::FixtureGroup* group)
    {

    }

    void setEffect(std::function<void()> eff)
    {
        m_effectFunction = eff;
    }

    void setCurveType(Utils::Curve::Type type)
    {
        m_curve.setType(type);
        m_curve.update();
        // m_curve.init();
    }

    void setCurvePeaks(uint16_t peaks)
    {
        m_curve.setPeaks(peaks);
        m_curve.update();
        // m_curve.init();
    }

    void setCurveLength(std::size_t length)
    {
        m_curve.setLength(length);
        m_curve.update();
        // m_curve.init();
    }

    void update()
    {
        m_effectFunction();
    }

};

struct ColorEffect : public Effect
{
    ColorEffect(DMX::FixtureGroup* group)
        : Effect(group)
    {
        m_parameters = group->getParameters(DMX::Parameters::Type::COLOR);
        m_curve.setLength(m_parameters.size());
    }
};
}