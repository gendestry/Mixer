//
// Created by bobi on 10/09/2025.
//

#include <iostream>
#include "DMX/FixtureGroup.cpp"
#include "DMX/Universe.h"


int main()
{
    using namespace DMX;
    Fixture ledbar("led");
    ledbar.addMultiple<Parameters::ColorRGB>(10);

    Fixture par("par");
    par.add<Parameters::Dimmer>();
    par.add<Parameters::ColorRGB>();

    Universe uni(1);
    Universe uni2(2);
    uni.addFixture(ledbar);
    uni2.addFixture(ledbar);
    uni.addFixture(par);
    uni.addFixture(par);
    uni.addFixture(par);
    uni.addFixture(par);

    FixtureGroup group;
    group += uni["led"];
    group += uni2["led"];
    group += uni["par"];

    for (auto fix : group()) {
        auto params = fix->getParameters(Parameters::ParameterTypes::COLOR).value();
        for (auto param : params) {
            param->setValue("B", 100.f);
        }
    }

    for (auto fix : uni["led"]) {
        auto params = fix->getParameters(Parameters::ParameterTypes::COLOR).value();
        for (auto param : params) {
            param->setValue("R", 25.f);
        }
    }

    std::cout << uni.describe();
    return 0;
}
