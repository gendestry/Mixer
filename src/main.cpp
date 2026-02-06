//
// Created by bobi on 10/09/2025.
//

#include <iostream>
#include "Engine/Engine.h"
#include <bits/this_thread_sleep.h>
#include "Helper/Utils.h"
#include "Helper/Colors.h"
#include "Helper/Curve.h"

int main()
{
    using namespace DMX;

    Engine engine("testfile.txt");
    engine.setIP("192.168.0.6");

    const auto fids1 = engine.patch("pixel", 8, 93);
    const auto fids2 = engine.patch("pixel", 9, 120);

    engine.addToGroup("ptop", fids1);
    engine.addToGroup("pside", fids2);
    engine.addToGroup("all", "ptop");
    engine.addToGroup("all", "pside");

    engine.addEffect2Color("all", {255, 0, 0}, {0, 0, 255});
    // engine.addColor("all", {255, 0, 0});
    auto* e = engine.addEffectDimmerChase("all");
    e->m_curve.setType(Utils::Curve::Type::SQUARE);
    e->m_curve.setPeaks(4);

    for (int i = 0; i < 500; i++) {
        engine.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        // std::this_thread::sleep_for(std::chrono::microseconds(16667));
    }

    std::cout << engine.describe();
    return 0;
}
