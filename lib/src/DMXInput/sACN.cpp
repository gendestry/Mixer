//
// Created by bobi on 13/08/2025.
//

#include "../../include/DMXInput/sACN.h"
#include <iostream>

namespace DMX::Input
{
    sACN::sACN(uint8_t universe)
        : m_universe(universe)
    {
        std::cout << "Connecting to universe: " << std::to_string(m_universe) << std::endl;
    }

    sACN::~sACN()
    {
        std::cout << "Disconnecting uni: " << std::to_string(m_universe) << std::endl;
    }

    // void connect([[maybe_unused]] uint8_t universe)
    // {
    //     disconnect();
    //
    // };

    void sACN::connect(uint8_t universe)
    {
        (void)universe;
    }

    void sACN::disconnect()
    {}

    // [[nodiscard]] uint8_t* sACN::getDmxBuffer()
    // {
    //     return m_dmxBuffer.data();
    // }
}
