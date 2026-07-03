#include "LightEngine/Engine/Engine.h"
#include "Utils/Network/Interfaces.h"
#include <chrono>
#include <iostream>
#include <thread>

using namespace LightEngine;

int main()
{
    Engine::Engine engine;

    // "pixel": RGB only (3 channels) -> gets a virtual dimmer when patched.
    engine.define("pixel").add(Parameters::Presets::ColorRGB());

    // Patch + group: group1 @ uni 8, group2 @ uni 9, group3 @ uni 10.
    engine.addToGroup("group1", engine.patch("pixel",  8,  93));
    engine.addToGroup("group2", engine.patch("pixel",  9, 120));
    engine.addToGroup("group3", engine.patch("pixel", 10,  60));

    auto& prog = engine.programmer();

    // Select group1 + group2, add a dimmer chase at 120 BPM.
    prog.select(*engine.getGroup("group1"));
    prog.addToSelection(*engine.getGroup("group2"));
    prog.addDimmerChase(60.0f);
    engine.storeCue("main", 1.0f);
    prog.clear();

    // New selection: group1 -> green.
    prog.select(*engine.getGroup("group1"));
    prog.setColor({0, 255, 0});
    prog.setIntensity(1.0f);
    engine.storeCue("main", 2.0f);
    prog.clear();

    // New selection: group3 -> intensity full.
    prog.select(*engine.getGroup("group3"));
    prog.setColor({0, 255, 255});
    prog.setIntensity(1.0f);
    engine.storeCue("main", 3.0f);

    std::cout << prog.describe() << '\n';

    // Store the programmer look as cue 1 in sequence "main", then clear the
    // programmer and recall the cue from playback.
    // engine.storeCue("main", 1.0f);
    std::cout << engine.sequence("main").describe() << '\n';
    prog.clear();
    engine.go("main");

    // Output + render loop ~40 fps.
    Utils::Network::Interfaces::scan();
    engine.setIP(Utils::Network::Interfaces::primaryIP());
    for (int frame = 0; frame < 400; ++frame)
    {
        if(frame == 150 || frame == 300)
        {
            engine.go("main");
        }
        engine.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }

    return 0;
}
