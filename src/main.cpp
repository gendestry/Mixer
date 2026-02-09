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

int main() {
    using namespace DMX;

    // Engine engine("testfile.txt");
    Engine engine;
    engine.setIP("192.168.0.6");

    Fixture fixture("pixel");
    fixture.add<Parameters::ColorRGB>();

    engine.addToGroup("all", engine.patch(fixture, 8, 93));
    engine.addToGroup("all", engine.patch(fixture, 9, 120));

    // auto* fx = engine.addEffectDimmerChase("all", Utils::Curve::SINUSOID);
    // fx->m_curve->setPeaks(4);
    // fx->m_curve->setMax(0.5f);
    engine.addDimmer("all", 100);
    // engine.addEffectDimmerGroup("all");
    // engine.addColor("all", {255, 128, 0});
    // auto f = engine.addEffect2Color("all", {255, 0, 0}, {0, 0, 255});
    // f->m_peaks = 3;
    engine.addEffectColorGradient("all", {Utils::Colors::Red, Utils::Colors::Yellow, Utils::Colors::Blue}, {0.2, 0.6, 0.2});
    for (int i = 0; i< 400; i++) {
        engine.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return 0;
}
