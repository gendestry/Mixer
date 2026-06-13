#include "Engine/Components/Patch.h"
#include "Engine/Components/DMXOutput.h"
#include "DMX/FixtureGroup.h"
#include "Utils/Network/Interfaces.h"
#include <iostream>

using namespace Core;

int main()
{
    Engine::Components::Patch patch;

    // "pixel": RGB only (3 channels). With colour but no dimmer it gets a
    // virtual dimmer automatically when patched.
    Fixture& pixel = patch.library().define("pixel");
    pixel.add(Parameters::Presets::ColorRGB());

    // Patch pixels across three universes:
    //   93 @ universe 8, 120 @ universe 9, 60 @ universe 10.
    auto u8  = patch.patch("pixel",  8,  93);
    auto u9  = patch.patch("pixel",  9, 120);
    auto u10 = patch.patch("pixel", 10,  60);

    // Group them all, colour at 50% via the virtual dimmer.
    DMX::FixtureGroup all("all");
    all.add(patch.getFixtures(u8));
    all.add(patch.getFixtures(u9));
    all.add(patch.getFixtures(u10));
    all.setColor({0, 128, 255});
    all.setIntensity(0.1f);
    all.applyVirtualDimmers();

    // Send the dirty universes (8, 9, 10 - marked dirty by patch()) over sACN.
    Engine::Components::DMXOutput output;
    Utils::Network::Interfaces::scan();
    output.setIP(Utils::Network::Interfaces::primaryIP());
    output.update(patch.dirtyUniverses(), patch);
    patch.clearDirty();

    // Inspect.
    std::cout << patch.describe() << "\n";
    std::cout << patch.getUniverse(8)->bytesToString() << '\n';

    return 0;
}
