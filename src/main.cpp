//
// Created by bobi on 10/09/2025.
//

#include <iostream>
#include "Engine/Engine.h"
// #include <chrono>
// #include <thread>
// #include "Helper/Colors.h"
// #include "Effect/EffectHolder.h"
// #include "DMX/Fixture/Fixture.h"
// #include "DMX/Fixture/Parameters/Parameters.h"

int main()
{
    using namespace DMX;

    Engine engine("testfile.txt");
    // engine.readFixtureLibrary("testfile.txt");
    engine.setIP("192.168.0.6");

    auto fids1 = engine.patch("pixel", 8, 93);
    auto fids2 = engine.patch("pixel", 9, 120);
    //
    engine.addToGroup("ptop", fids1);
    engine.addToGroup("pside", fids2);
    engine.addToGroup("all", "ptop");
    engine.addToGroup("all", "pside");
    //
    // {
    //     // auto& params = engine.getGroupParameter("ledbars", Parameters::Type::DIMMER);
    //     // if (params.size() == 0)
    //     // {
    //     //     auto& cparams = engine.getGroupParameter("ledbars", Parameters::Type::COLOR);
    //     //     for (auto param : cparams)
    //     //     {
    //     //         param->se
    //     //     }
    //     // }
    // }
    //
    {
        auto &params = engine.getGroupColorParameter("all");
        //     // int i = 0;
        for (auto param : params)
        {
            auto perc = 0.f;
            // auto perc = ++i / static_cast<float>(params.size()) * 100.f;
            param->setValue("R", 100.f - perc);
            param->setValue("G", 100.f - perc);
            param->setValue("B", 0.f - perc);
        }
        //
        engine.addDirtyUniverse("all");
        engine.update();
    }
    {
        // engine.m_programmer.setGroup(&engine.getFixtureGroup("ledbars"));
        // engine.m_programmer.addEffect<Effect::ColorEffect>([](){});
        // engine.m_programmer.setCurveType(Utils::Curve::SINUSOID);

        // auto& params = engine.getGroupParameter("ledbars", Parameters::Type::COLOR);
        // engine.addDirtyUniverse("ledbars");
        // // auto grad = Utils::getGradient(5, c, {50.f, 50.f});
        // auto interp = Utils::linearInterpolation(0, 100.f, params.size());

        // auto r = Utils::Colors::rgbToHsv({255, 0, 0});
        // auto b = Utils::Colors::rgbToHsv({0, 0, 255});
        // auto hs = Utils::linearInterpolation(r[0], b[0], 120);

        // for(int x = 0; x < 100; x++)
        // {
        //     int i = 0;
        //     for (auto param : params)
        //     {
        //         int offs = (x + i) % params.size();
        //         auto col = Utils::Colors::hsvToRgb({hs[offs], i / 120.f, 1});

        //         param->setValue("R", (col[0] /255.f) * 100.f);
        //         param->setValue("B", (col[2] /255.f) * 100.f);
        //         i++;
        //     }

        //     std::this_thread::sleep_for(std::chrono::milliseconds{50});
        //     engine.update();
        // }
    }
    // engine.setFixtureID(201, 301);
    // {
    //     auto params = engine.getFixtureByFID(301)->getParameters(Parameters::Type::COLOR);
    //     if (params.has_value())
    //     {
    //         for (auto f : params.value())
    //         {
    //             f->setValue("G", 100.f);
    //         }
    //     }
    // }

    std::cout << engine.describe();

    return 0;
}
