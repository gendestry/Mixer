#pragma once
#include "DMX/FixtureGroup.h"
#include "Effect/Effect.h"
#include "../Helper/Curve.h"
#include "DMX/Fixture/Parameters/Parameters.h"

class Programmer
{
protected:
    DMX::FixtureGroup* selectedGroup;
    Effect::Effect* currentEffect;

public:
    void setGroup(DMX::FixtureGroup* other)
    {
        selectedGroup = other;
    }

    template <typename TEffect>
    void addEffect(std::function<void()> eff)
    {
        currentEffect = new TEffect(selectedGroup);
        currentEffect->setEffect(eff);
        // selectedGroup->getParameters(type);
    };

    void setCurveType(Utils::Curve::Type type)
    {
        currentEffect->setCurveType(type);
    }

    void setCurvePeaks(uint16_t peaks)
    {
        currentEffect->setCurvePeaks(peaks);
    }

    void setCurveLength(std::size_t length)
    {
        currentEffect->setCurveLength(length);
    }



    void appendToGroup();
    
    void update();
    void clear()
    {
        
    }
};