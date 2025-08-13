//
// Created by bobi on 13/08/2025.
//

#pragma once
#include <array>
#include <cstdint>

namespace DMX::Output
{

enum class LedOrder
{
    RGB,
    GRB,
    RGBW
};

template<LedOrder TOrder>
struct OrderToNum
{
    static constexpr uint8_t amount = 3;
};

template<>
struct OrderToNum<LedOrder::RGBW>
{
    static constexpr uint8_t amount = 4;
};

template<uint16_t TLedNum, LedOrder TLedOrder>
class OutputWrapper
{
    static constexpr uint16_t k_numBytesPerLed = OrderToNum<TLedOrder>::amount;
    static constexpr uint16_t k_byteSize = TLedNum * k_numBytesPerLed;

    std::array<uint8_t, k_byteSize> m_ledBytes;
public:
    virtual void update() = 0;
    uint8_t* getLedBytes() { return m_ledBytes.data(); }
};

// template<uint8_t TLedNum, LedOrder TLedOrder, template<typename...> class TOutputWrapper>
template<uint8_t TLedNum, LedOrder TLedOrder, uint8_t TPin>
struct FastLedOutput : public OutputWrapper<TLedNum, TLedOrder>
{

    void update() override
    {

    }
};



// template<uint8_t TLedNum, LedOrder TLedOrder, template<typename...> class TOutputDerived, typename... Args>
// std::unique_ptr<OutputWrapper<TLedNum, TLedOrder>> make_output_wrapper(Args&&... args)
// {
//     return std::make_unique<
// }

}

