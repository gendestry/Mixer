//
// Created by bobi on 10/09/2025.
//

#include <iostream>
#include "Engine/Engine.h"
#include <bits/this_thread_sleep.h>
#include "Helper/Utils.h"
#include "Helper/Colors.h"
#include "Helper/Curve.h"
#include "Helper/NetworkUtils.h"
#include <print>

int main()
{
    using namespace DMX;

    // Engine engine("testfile.txt");
    Engine engine;
    engine.setIP(NetworkUtils::primaryIP());

    Fixture fixture("pixel");
    fixture.add<Parameters::ColorRGB>();

    engine.addToGroup("all", engine.patch(fixture, 8, 93));
    engine.addToGroup("all", engine.patch(fixture, 9, 120));

    // engine.add
    auto* fx = engine.addEffectDimmerChase("all", Utils::Curve::SINUSOID);
    // Utils::Sinusoid s(10, 1);
    // auto* fx = engine.addEffectDimmerChase("all", std::move(s));
    // fx->m_curve.setPeaks(4);
    // engine.addDimmer("all", 100);
    // engine.addEffectDimmerGroup("all");
    // engine.addColor("all", {255, 128, 0});
    engine.addEffect2Color("all", {255, 0, 0}, {0, 0, 255});
    // std::vector<Utils::Colors::RGB> colors;
    // colors.push_back(Utils::Colors::RGB(255, 0, 0));
    // colors.push_back(Utils::Colors::RGB(255, 128, 0));
    // colors.push_back(Utils::Colors::RGB(0, 0, 255));

    // std::vector<float> percentages{0.2, 0.6, 0.2};
    // engine.addEffectColorGradient("all", colors, percentages);
    for (int i = 0; i< 400; i++) {
      engine.update();
      std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    // auto params = engine.getGroupColorParameter("all");
    // const auto grad = Utils::Colors::makeGradient({{255, 0, 0}, {0, 0, 255}}, {0.5f, 0.5f}, params.size());
    // int counter = 0;
    // for (auto& param : params)
    // {
    //     auto hsv = Utils::Colors::rgbToHsv(grad[counter++]);
    //     Utils::Colors::HSV hsvParam = Utils::Colors::rgbToHsv(
    //     { static_cast<uint8_t>(param->getValue("R").value() * 255.f),
    //     static_cast<uint8_t>(param->getValue("G").value() * 255.f),
    //     static_cast<uint8_t>(param->getValue("B").value() * 255.f)});
    //
    //     // std::println("{} {} {}", hsvParam.h, hsvParam.s, hsvParam.v);
    //     hsv.v = hsvParam.v;
    //     auto rgb = Utils::Colors::hsvToRgb(hsv);
    //     param->setValue("R", rgb.pr);
    //     param->setValue("G", rgb.pg);
    //     param->setValue("B", rgb.pb);
    // }
    //
    // engine.update();

     // setEffect([this, color](uint32_t tick)
     //    {
     //        Utils::Colors::HSV hsv = Utils::Colors::rgbToHsv({color.r, color.g, color.b});
     //
     //        for (auto& param : this->m_parameters)
     //        {
     //            Utils::Colors::HSV hsvParam = Utils::Colors::rgbToHsv(
     //                {static_cast<uint8_t>(param->getValue("R").value() / 100.f * 255.f),
     //                static_cast<uint8_t>(param->getValue("G").value() / 100.f * 255.f),
     //                static_cast<uint8_t>(param->getValue("B").value() / 100.f * 255.f)});
     //
     //            hsv.v = hsvParam.v;
     //            Utils::Colors::RGB c = Utils::Colors::hsvToRgb(hsv);
     //
     //            param->setValue("R", c.pr);
     //            param->setValue("G", c.pg);
     //            param->setValue("B", c.pb);
     //        }
     //    });

     // std::cout << engine.describe();
     return 0;
}
