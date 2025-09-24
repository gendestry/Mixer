//
// Created by bobi on 10/09/2025.
//

#include <iostream>
#include "DMX/FixtureGroup.h"
#include "DMX/Universe.h"
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
    engine.patch(par, 1, 5, std::nullopt, 102);

    FixtureGroup group("group1");
    group += engine.getUniverse(1)["led"];
    group += engine.getUniverse(2)["led"];
    group += engine.getUniverse(1)["par"];
    //
    FixtureGroup ledbarGroup("ledbars");
    ledbarGroup += engine.getUniverse(1)["led"];
    ledbarGroup += engine.getUniverse(2)["led"];

    engine.addFixtureGroup(group);
    engine.addFixtureGroup(ledbarGroup);
    engine.addToGroup(0, {101, 201, 102, 103, 104, 105, 106});
    engine.addToGroup(1, {101, 201});
    //
    //
    for (auto color : engine.getFixtureGroup(0).getParameters(Parameters::ParameterTypes::COLOR))
    {
        color->setValue("B", 100.f);
    }

    for (auto param : engine.getFixtureGroup(1).getParameters(Parameters::ParameterTypes::COLOR)) {
        param->setValue("R", 25.f);
    }


    std::cout << engine.getUniverse(1).describe();
    std::cout << engine.getUniverse(2).describe();

    return 0;
}
