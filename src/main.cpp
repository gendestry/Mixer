//
// Created by bobi on 10/09/2025.
//

#include <iostream>
#include "Engine/Engine.h"
#include <bits/this_thread_sleep.h>
#include "Helper/Utils.h"
#include "Helper/Colors.h"
#include "Helper/Curve.h"
#include <fmt/format.h>
#include <print>
#include <source_location>
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
    engine.setIP("192.168.0.6");

    const auto fids1 = engine.patch("pixel", 8, 93);
    const auto fids2 = engine.patch("pixel", 9, 120);

    engine.addToGroup("ptop", fids1);
    engine.addToGroup("pside", fids2);
    engine.addToGroup("all", "ptop");
    // engine.addToGroup("all", "pside");

    const auto &params = engine.getGroupColorParameter("all");
    std::vector<Utils::Colors::RGB> colors { {255, 0, 0},{255, 128, 0},  {0, 0, 255}};
    std::vector<float> perc { 0.33f, 0.60f, 0.2f};
    auto grad = Utils::Colors::makeGradient(colors, perc, params.size());
    for (int i = 0; i < params.size(); ++i)
    {
        auto& color = grad[i];
        auto param = params[i];
        param->setValue("R", color.pr);
        param->setValue("G", color.pg);
        param->setValue("B", color.pb);
    }

    // engine.update();


/*        const auto &params = engine.getGroupColorParameter("all");
        const auto inter = Utils::linearInterpolation(0.f, 1.f, params.size(), true);
        const Utils::Curve curve(Utils::Curve::Type::TRIANGLE,params.size(), 1);

        std::vector<Utils::Colors::RGB> setpoints{
            {255, 0, 0},
            {128, 128, 0},
            {0, 0, 255},
            {0, 255, 0},
            {20, 20, 20}
        };

    size_t numSteps  = params.size();
    size_t numPoints = setpoints.size();
    size_t numSegments = numPoints; // 0->1, 1->2, 2->0

        for (size_t i = 0; i < params.size(); ++i) {
            // Overall progress [0, 1)
            float progress = static_cast<float>(i) / numSteps;

            // Which segment are we in?
            float segmentPos = progress * numSegments;
            size_t segment = static_cast<size_t>(segmentPos);

            // Interpolation inside segment [0, 1]
            float t = segmentPos - segment;

            // Wrap indices
            size_t i1 = segment % numPoints;
            size_t i2 = (i1 + 1) % numPoints;

            const auto& c1 = setpoints[i1];
            const auto& c2 = setpoints[i2];

            Utils::Colors::RGB color{
                static_cast<uint8_t>(c1.r + t * (c2.r - c1.r)),
                static_cast<uint8_t>(c1.g + t * (c2.g - c1.g)),
                static_cast<uint8_t>(c1.b + t * (c2.b - c1.b))
            };

            params[i]->setValue("R", color.pr);
            params[i]->setValue("G", color.pg);
            params[i]->setValue("B", color.pb);
        }
    engine.update();*/


        // for (int i = 0; i < 1000; i++)
        // {
        //     int o = i;
        //     for (const auto& param : params)
        //     {
        //         Utils::Colors::HSV hsv(inter[o % inter.size()], 1.f, 1.f);
        //         const auto color = Utils::Colors::hsvToRgb(hsv).toPercentage();
        //
        //         param->setValue("R", color.pr);
        //         param->setValue("G", color.pg);
        //         param->setValue("B", color.pb);
        //         o++;
        //     }
        //     engine.update();
        //     std::this_thread::sleep_for(std::chrono::milliseconds(20));
        // }
    // }

    std::cout << engine.describe();

    return 0;
}
