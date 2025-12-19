//
// Created by bobi on 10/09/2025.
//

#include <iostream>
#include "Engine/Engine.h"
#include <chrono>
#include <thread>
#include "Helper/Colors.h"

int main()
{
    using namespace DMX;

    Engine engine("testfile.txt");
    engine.patch("ledbar", 8, 1);;
    // engine.patch("ledbar", 9, 1);
    // auto par_fids = engine.patch("par", 1, 5, std::nullopt, 102);
    //
    // engine.addToGroup("mix", {101, 201, 102, 103, 104, 105, 106});
    engine.addToGroup("ledbars", engine.getFixturesFIDByName("ledbar"));
    // engine.addToGroup("pars", par_fids);


    {
        // auto& params = engine.getGroupParameter("ledbars", Parameters::Type::DIMMER);
        // if (params.size() == 0)
        // {
        //     auto& cparams = engine.getGroupParameter("ledbars", Parameters::Type::COLOR);
        //     for (auto param : cparams)
        //     {
        //         param->se
        //     }
        // }
    }

    // {
    //     auto& params = engine.getGroupParameter("mix", Parameters::Type::COLOR);
    //     int i = 0;
    //     for (auto param : params)
    //     {
    //         auto perc = ++i / static_cast<float>(params.size()) * 100.f;
    //         param->setValue("B", 100.f - perc);
    //     }
    // }

    {
        auto& params = engine.getGroupParameter("ledbars", Parameters::Type::COLOR);
        engine.addDirtyUniverse("ledbars");
        // auto grad = Utils::getGradient(5, c, {50.f, 50.f});
        auto interp = Utils::linearInterpolation(0, 100.f, params.size());

        auto r = Utils::Colors::rgbToHsv({255, 0, 0});
        auto b = Utils::Colors::rgbToHsv({0, 0, 255});
        auto hs = Utils::linearInterpolation(r[0], b[0], 60);

        for(int x = 0; x < 100; x++)
        {
            int i = 0;
            for (auto param : params)
            {
                int offs = (x + i) % params.size();
                auto col = Utils::Colors::hsvToRgb({hs[offs], i / 60.f, 1});

                param->setValue("R", (col[0] /255.f) * 100.f);
                param->setValue("B", (col[2] /255.f) * 100.f);
                i++;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds{50});
            engine.update();
        }
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
