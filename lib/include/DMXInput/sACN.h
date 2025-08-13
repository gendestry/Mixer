#pragma once

#include <array>
#include <cstdint>

#include "Traits/Connectable.h"

namespace DMX::Input
{

class sACN : public Traits::Connectable
{
    uint8_t m_universe;
public:
    // sACN() = delete;
    sACN(uint8_t universe);
    ~sACN();

    // void setUniverse();

    // void connect([[maybe_unused]] uint8_t universe)
    // {
    //     disconnect();
    //
    // };

    void connect(uint8_t universe) override;
    void disconnect() override;
};
};



