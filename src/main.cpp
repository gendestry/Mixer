//
// Created by bobi on 10/09/2025.
//

#include <iostream>
#include "Engine/Engine.h"

int main()
{
    using namespace DMX;

    Engine engine("testfile.txt");
    engine.patch("ledbar", 1, 1);;
    engine.patch("ledbar", 2, 1);
    auto par_fids = engine.patch("par", 1, 5, std::nullopt, 102);
    //
    engine.addToGroup("mix", {101, 201, 102, 103, 104, 105, 106});
    engine.addToGroup("ledbars", engine.getFixturesFIDByName("ledbar"));
    engine.addToGroup("pars", par_fids);


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

    {
        auto& params = engine.getGroupParameter("mix", Parameters::Type::COLOR);
        int i = 0;
        for (auto param : params)
        {
            auto perc = ++i / static_cast<float>(params.size()) * 100.f;
            param->setValue("B", 100.f - perc);
        }
    }
    {
        auto& params = engine.getGroupParameter("ledbars", Parameters::Type::DIMMER);
        int i = 0;
        for (auto param : params)
        {
            auto perc = ++i / static_cast<float>(params.size()) * 100.f;
            param->setValue("R", perc);
        }
    }
    //
    //
    //
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
