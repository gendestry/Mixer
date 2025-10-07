#pragma once
#include <vector>
#include <string>
#include <cstdint>

namespace Utils
{

    std::vector<uint8_t> toBytes(int numBytes, int number);
    int toNumber(std::vector<uint8_t> bytes);
    std::string padByte(int num, int pad);
    std::string colorByRGB(int r, int g, int b, bool fg);
    std::vector<float> linearInterpolation(float start, float end, int n);

    std::vector<float> rgbToHsv(std::vector<int> rgb);
    std::vector<int> hsvToRgb(std::vector<float> hsv);

    std::vector<std::vector<int>> getGradient(int numElements, std::vector<std::vector<int>> colors, std::vector<float> percentages = {});

};