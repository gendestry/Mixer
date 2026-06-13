#include "Engine/Components/Patch.h"
#include "DMX/FixtureGroup.h"
#include <iostream>

using namespace Core;

int main()
{
    Engine::Components::Patch patch;

    // --------------------------------------------------------------------
    // 1. Define fixture personalities in the library.
    // --------------------------------------------------------------------
    // "par": a real dimmer + RGB (4 channels). setIntensity drives the
    // dimmer channel directly.
    Fixture& par = patch.library().define("par");
    par.add(Parameters::Presets::Dimmer());
    par.add(Parameters::Presets::ColorRGB());

    // "pixel": RGB only (3 channels). Having color but no dimmer, it gets a
    // virtual dimmer automatically when patched - intensity is applied by
    // scaling the colour in HSV space.
    Fixture& pixel = patch.library().define("pixel");
    pixel.add(Parameters::Presets::ColorRGB());

    std::cout << patch.library().describe() << "\n\n";

    // --------------------------------------------------------------------
    // 2. Patch fixtures into universe 1.
    // --------------------------------------------------------------------
    auto parFids   = patch.patch("par",   /*universe*/ 1, /*amount*/ 4); // FIDs 1..4  @ ch 0,4,8,12
    auto pixelFids = patch.patch("pixel", /*universe*/ 1, /*amount*/ 3); // FIDs 5..7  @ ch 16,19,22

    std::cout << "par FIDs:   ";
    for (auto f : parFids)   std::cout << f << ' ';
    std::cout << "\npixel FIDs: ";
    for (auto f : pixelFids) std::cout << f << ' ';
    std::cout << "\n\n";

    // --------------------------------------------------------------------
    // 3a. Group the pars; red at 50% via the REAL dimmer channel.
    // --------------------------------------------------------------------
    DMX::FixtureGroup pars("pars");
    pars.add(patch.getFixtures(parFids));
    pars.setColor({255, 0, 0});
    pars.setIntensity(0.5f);          // writes the dimmer channel to 128

    // --------------------------------------------------------------------
    // 3b. Group the pixels; cyan-ish at 50% via the VIRTUAL dimmer.
    // --------------------------------------------------------------------
    DMX::FixtureGroup pixels("pixels");
    pixels.add(patch.getFixtures(pixelFids));
    pixels.setColor({0, 128, 255});
    pixels.setIntensity(0.5f);        // stores the virtual-dimmer level only
    pixels.applyVirtualDimmers();     // composes colour * level into the buffer

    // --------------------------------------------------------------------
    // 4. Inspect.
    // --------------------------------------------------------------------
    std::cout << pars.describe()   << "\n\n";
    std::cout << pixels.describe() << "\n\n";
    std::cout << patch.getUniverse(1)->describe() << '\n';
    std::cout << patch.getUniverse(1)->bytesToString() << '\n';

    return 0;
}
