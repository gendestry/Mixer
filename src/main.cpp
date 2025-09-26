//
// Created by bobi on 10/09/2025.
//

#include <iostream>
#include "Engine/Engine.h"
#include "Engine/FixtureLibrary.h"


int main()
{
    using namespace DMX;

    FixtureLibrary fixLib;
    {
        Fixture ledbar("led");
        ledbar.addMultiple<Parameters::ColorRGB>(10);

        Fixture par("par");
        par.add<Parameters::Dimmer>();
        par.add<Parameters::ColorRGB>();

        fixLib.add(ledbar);
        fixLib.add(par);
    }


    auto ledbar = fixLib.get("led").value();
    auto par = fixLib.get("par").value();

    Engine engine;
    engine.patch(ledbar, 1, 1);;
    engine.patch(ledbar, 2, 1);
    auto par_fids = engine.patch(par, 1, 5, std::nullopt, 102);

    engine.addToGroup("mix", {101, 201, 102, 103, 104, 105, 106});
    engine.addToGroup("ledbars", engine.getFixturesFIDByName("led"));
    engine.addToGroup("pars", par_fids);

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



    engine.setFixtureID(201, 301);
    {
        auto params = engine.getFixtureByFID(301)->getParameters(Parameters::Type::COLOR);
        if (params.has_value())
        {
            for (auto f : params.value())
            {
                f->setValue("G", 100.f);
            }
        }
    }

    std::cout << engine.getUniverse(1).describe();
    std::cout << engine.getUniverse(2).describe();

    return 0;
}
