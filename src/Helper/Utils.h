#pragma once
#include <vector>
#include <string>
#include <cstdint>

namespace Utils
{

    std::vector<uint8_t> toBytes(int numBytes, int number);
    int toNumber(std::vector<uint8_t> bytes);
    std::string padByte(int num, int pad);

    std::vector<float> linearInterpolation(float start, float end, uint32_t n, bool mirror = false);
    std::vector<uint16_t> make_range(uint16_t start, uint16_t end);

    // std::vector<std::vector<int>> getGradient(int numElements, std::vector<std::vector<int>> colors, std::vector<float> percentages = {});

};