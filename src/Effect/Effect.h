#pragma once
#include <functional>
#include "DMX/FixtureGroup.h"
#include "DMX/Fixture/Parameters/Parameters.h"
#include "Helper/Curve.h"
#include <variant>

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
    uint16_t bpm = 60;

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

    void setBPM(uint16_t bpm) {
        this->bpm = bpm;
    }

    void update()
    {
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

};

struct ColorEffect : public Effect
{
    ColorEffect(DMX::FixtureGroup& group, Utils::Colors::RGB color)
        : Effect(group, Type::COLOR)
    {

        setEffect([this, color](uint32_t tick)
        {
            auto hsv = Utils::Colors::rgbToHsv(color);
            for (auto& param : this->m_parameters)
            {
                auto r = static_cast<uint8_t>(param->getValue("R").value() * 255.f);
                auto g = static_cast<uint8_t>(param->getValue("G").value() * 255.f);
                auto b = static_cast<uint8_t>(param->getValue("B").value() * 255.f);
                Utils::Colors::HSV hsvParam = Utils::Colors::rgbToHsv({r, g, b});

                hsv.v = hsvParam.v;
                auto rgb = Utils::Colors::hsvToRgb(hsv);
                param->setValue("R", rgb.pr);
                param->setValue("G", rgb.pg);
                param->setValue("B", rgb.pb);
            }
        });
    }
};

struct FX2Color : public Effect
{
    uint16_t m_peaks;
    FX2Color(DMX::FixtureGroup& group, Utils::Colors::RGB color1, Utils::Colors::RGB color2, uint16_t peaks = 1)
        : Effect(group, Type::COLOR),
            m_peaks(peaks)

    {
        setEffect([color1, color2, this](uint32_t tick) {
            auto& params = this->m_parameters;
            const auto grad = Utils::Colors::makeGradient({color1, color2}, {0.5f, 0.5f}, params.size() / m_peaks);
            for (int i = 0; i < params.size(); ++i)
            {
                auto& color = grad[(i + tick) % (params.size() / m_peaks)];
                const auto& param = params[i];
                auto hsv = Utils::Colors::rgbToHsv(color);

                auto r = static_cast<uint8_t>(param->getValue("R").value() * 255.f);
                auto g = static_cast<uint8_t>(param->getValue("G").value() * 255.f);
                auto b = static_cast<uint8_t>(param->getValue("B").value() * 255.f);
                Utils::Colors::HSV hsvParam = Utils::Colors::rgbToHsv({r, g, b});

                hsv.v = hsvParam.v;
                auto rgb = Utils::Colors::hsvToRgb(hsv);
                param->setValue("R", rgb.pr);
                param->setValue("G", rgb.pg);
                param->setValue("B", rgb.pb);
            }
        });
    }
};

struct FXColorGradient : public Effect
{
    FXColorGradient(DMX::FixtureGroup& group, const std::vector<Utils::Colors::RGB>& colors, const std::vector<float>& percentages)
        : Effect(group, Type::COLOR)
    {
        setEffect([colors, percentages, this](uint32_t tick) {
            auto& params = this->m_parameters;
            const auto grad = Utils::Colors::makeGradient(colors, percentages, params.size());
            for (int i = 0; i < params.size(); ++i)
            {
                auto& color = grad[(i + tick) % params.size()];
                const auto& param = params[i];

                auto r = static_cast<uint8_t>(param->getValue("R").value() * 255.f);
                auto g = static_cast<uint8_t>(param->getValue("G").value() * 255.f);
                auto b = static_cast<uint8_t>(param->getValue("B").value() * 255.f);
                Utils::Colors::HSV hsvParam = Utils::Colors::rgbToHsv({r, g, b});

                auto hsv = Utils::Colors::rgbToHsv(color);
                hsv.v = hsvParam.v;

                auto rgb = Utils::Colors::hsvToRgb(hsv);
                param->setValue("R", rgb.pr);
                param->setValue("G", rgb.pg);
                param->setValue("B", rgb.pb);
            }
        });
    }
};

struct DimmerEffect : public Effect
{
    DimmerEffect(DMX::FixtureGroup& group, float dimmer)
        : Effect(group, Type::DIMMER)
    {
        setEffect([this, dimmer](uint32_t tick)
        {
            auto dim = dimmer / 100.f;
            for (auto& param : this->m_parameters)
            {
                auto r = static_cast<uint8_t>(param->getValue("R").value() * 255.f);
                auto g = static_cast<uint8_t>(param->getValue("G").value() * 255.f);
                auto b = static_cast<uint8_t>(param->getValue("B").value() * 255.f);
                Utils::Colors::HSV hsvParam = Utils::Colors::rgbToHsv({r, g, b});

                hsvParam.v = dim;
                auto rgb = Utils::Colors::hsvToRgb(hsvParam);
                param->setValue("R", rgb.pr);
                param->setValue("G", rgb.pg);
                param->setValue("B", rgb.pb);
            }
        });
    }
};

struct FXDimmerChase : public Effect
{
    std::unique_ptr<Utils::Curve::Interface> m_curve;

    FXDimmerChase(DMX::FixtureGroup& group, const Utils::Curve::Type type = Utils::Curve::SINUSOID)
        : Effect(group, Type::DIMMER),
          m_curve(Utils::Curve::getCurveByType(type, m_parameters.size()))
    {
        // std::visit([this](auto& c){ c.setLength(this->m_parameters.size()); }, m_curve);
        bpm = 2;

        setEffect([this](uint32_t tick) {
            auto& params = this->m_parameters;
            for (int i = 0; i < params.size(); ++i)
            {
                auto param = params[i];
                auto r = static_cast<uint8_t>(param->getValue("R").value() * 255.f);
                auto g = static_cast<uint8_t>(param->getValue("G").value() * 255.f);
                auto b = static_cast<uint8_t>(param->getValue("B").value() * 255.f);

                auto hsv = Utils::Colors::rgbToHsv({r,g,b});
                // hsv.v = std::visit([tick, i](auto& c){ return c[tick + i]; }, this->m_curve);
                hsv.v = this->m_curve->operator[](tick + i);

                auto color = Utils::Colors::hsvToRgb(hsv);
                param->setValue("R", color.pr);
                param->setValue("G", color.pg);
                param->setValue("B", color.pb);
            }
        });
    }
};

struct FXDimmerGrouped : public Effect
{
    // Utils::Curve::Interface m_curve;
    std::unique_ptr<Utils::Curve::Interface> m_curve;
    FXDimmerGrouped(DMX::FixtureGroup& group, const Utils::Curve::Type type = Utils::Curve::SINUSOID)
        : Effect(group, Type::DIMMER),
          m_curve(Utils::Curve::getCurveByType(type, m_parameters.size()))
    {
        setEffect([this](uint32_t tick) {
            auto& params = this->m_parameters;
            for (auto& param : params)
            {
                auto r = static_cast<uint8_t>(param->getValue("R").value() * 255.f);
                auto g = static_cast<uint8_t>(param->getValue("G").value() * 255.f);
                auto b = static_cast<uint8_t>(param->getValue("B").value() * 255.f);

                auto hsv = Utils::Colors::rgbToHsv({r,g,b});
                hsv.v = this->m_curve->operator[](tick);

                auto color = Utils::Colors::hsvToRgb(hsv);
                param->setValue("R", color.pr);
                param->setValue("G", color.pg);
                param->setValue("B", color.pb);
            }
        });
    }
};
}