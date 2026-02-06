#pragma once
#include <functional>
#include "DMX/FixtureGroup.h"
#include "DMX/Fixture/Parameters/Parameters.h"
#include "Helper/Curve.h"

namespace Effect
{

    using EffectFunction = std::function<void(uint32_t)>;
// using EffectFunction = std::function<void(uint32_t, std::vector<std::shared_ptr<DMX::Parameters::Parameter>>&)>;

class Effect 
{
public:
    enum Type
    {
        DIMMER,
        COLOR
    };
protected:
    // DMX::FixtureGroup* m_group;
    std::vector<std::shared_ptr<DMX::Parameters::Parameter>> m_parameters; 
    EffectFunction m_effectFunction;
    Type m_type;
    uint32_t tick = 0;
    uint32_t engineTick = 1;
    uint16_t bpm = 1;

    // Utils::Curve m_curve;

    // virtual void updateImpl() = 0;
public:


    struct Config
    {
    };

    Effect(DMX::FixtureGroup& group, Type type)
    {
        m_parameters = group.getParameters(DMX::Parameters::Type::COLOR);
        m_type = type;
    }

    void setEffect(EffectFunction eff)
    {
        m_effectFunction = std::move(eff);
    }

    void update()
    {
        // auto bps = bpm / 60.f;
        // float et = engineTick++ * bps;
        // if (et >= 60.f)
        // {
            m_effectFunction(tick++);
        //     engineTick = engineTick % 60;
        // }
        // auto x = et / 60.f; // ebps
        // if (et * 60.f)

    }

};

struct ColorEffect : public Effect
{
    // ColorEffect(DMX::FixtureGroup& group)
    //     : Effect(group, Type::COLOR)
    // {
    //     // m_curve.setLength(m_parameters.size());
    // }

    ColorEffect(DMX::FixtureGroup& group, Utils::Colors::RGB color)
        : Effect(group, Type::COLOR)
    {

        setEffect([this, color](uint32_t tick)
        {
            for (auto& param : this->m_parameters)
            {
                param->setValue("R", color.pr);
                param->setValue("G", color.pg);
                param->setValue("B", color.pb);
            }
        });
    }
};

struct FX2Color : public Effect
{
    FX2Color(DMX::FixtureGroup& group, Utils::Colors::RGB color1, Utils::Colors::RGB color2)
        : Effect(group, Type::COLOR)
    {
        setEffect([color1, color2, this](uint32_t tick) {
            auto& params = this->m_parameters;
            const auto grad = Utils::Colors::makeGradient({color1, color2}, {0.5f, 0.5f}, params.size());
            for (int i = 0; i < params.size(); ++i)
            {
                auto& color = grad[(i + tick) % params.size()];
                const auto& param = params[i];

                param->setValue("R", color.pr);
                param->setValue("G", color.pg);
                param->setValue("B", color.pb);
            }
        });
    }
};

struct FXDimmerChase : public Effect
{
    Utils::Curve m_curve;
    FXDimmerChase(DMX::FixtureGroup& group, const Utils::Curve::Type type = Utils::Curve::SINUSOID)
        : Effect(group, Type::DIMMER),
          m_curve(Utils::Curve(type, m_parameters.size()))
    {
        setEffect([this](uint32_t tick) {
            auto& params = this->m_parameters;
            for (int i = 0; i < params.size(); ++i)
            {
                auto param = params[i];
                auto r = static_cast<uint8_t>(param->getValue("R").value() * 255.f);
                auto g = static_cast<uint8_t>(param->getValue("G").value() * 255.f);
                auto b = static_cast<uint8_t>(param->getValue("B").value() * 255.f);

                auto hsv = Utils::Colors::rgbToHsv({r,g,b});
                hsv.v = this->m_curve[(tick + i)];

                auto color = Utils::Colors::hsvToRgb(hsv);
                param->setValue("R", color.pr);
                param->setValue("G", color.pg);
                param->setValue("B", color.pb);
            }
        });
    }
};
}