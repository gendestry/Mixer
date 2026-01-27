#pragma once
#include <vector>
#include <string>
#include <cstdint>

namespace Utils
{

    std::vector<uint8_t> toBytes(int numBytes, int number);
    int toNumber(std::vector<uint8_t> bytes);
    std::string padByte(int num, int pad);

    const std::vector<uint16_t> make_range(uint16_t start, uint16_t end);
    // std::string colorByRGB(int r, int g, int b, bool fg);
    std::vector<float> linearInterpolation(float start, float end, int n);

    // struct Range
    // {
    //     uint32_t begin;
    //     uint32_t end;

    //     Range()
    // };

    // std::vector<float> rgbToHsv(std::vector<int> rgb);
    // std::vector<int> hsvToRgb(std::vector<float> hsv);

    // std::vector<std::vector<int>> getGradient(int numElements, std::vector<std::vector<int>> colors, std::vector<float> percentages = {});

};