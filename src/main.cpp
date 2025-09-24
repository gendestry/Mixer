//
// Created by bobi on 10/09/2025.
//

#include <iostream>
#include "Engine/Engine.h"


int main()
{
    using namespace DMX;
    Fixture ledbar("led");
    ledbar.addMultiple<Parameters::ColorRGB>(10);

    Fixture par("par");
    par.add<Parameters::Dimmer>();
    par.add<Parameters::ColorRGB>();

    Engine engine;
    engine.patch(ledbar, 1, 1);;
    engine.patch(ledbar, 2, 1);
    auto par_fids = engine.patch(par, 1, 5, std::nullopt, 102);

    engine.addToGroup("mix", {101, 201, 102, 103, 104, 105, 106});
    engine.addToGroup("ledbars", {101, 201});
    engine.addToGroup("pars", par_fids);


    // {
    //     auto& params = engine.getGroupParameter("mix", Parameters::ParameterTypes::COLOR);
    //     int i = 0;
    //     for (auto param : params)
    //     {
    //         auto perc = i / static_cast<float>(params.size());
    //         param->setValue("B", perc * i++);
    //     }
    // }

    {
        auto& params = engine.getGroupParameter("pars", Parameters::ParameterTypes::COLOR);
        int i = 0;
        for (auto param : params)
        {
            auto perc = ++i / static_cast<float>(params.size()) * 100.f;
            param->setValue("R", perc);
        }
    }


    std::cout << engine.getUniverse(1).describe();

    return 0;
}
