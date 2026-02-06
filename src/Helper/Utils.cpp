//
// Created by bobi on 15/09/2025.
#include "Utils.h"
#include <sstream>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include <ranges>

namespace Utils
{

    std::vector<uint8_t> toBytes(int numBytes, int number)
    {
        std::vector<uint8_t> bytes(numBytes);
        for (int i = 0; i < numBytes; i++)
        {
            bytes[numBytes - i - 1] = (number >> (i * 8)) & 0xFF;
        }
        return bytes;
    }

    int toNumber(std::vector<uint8_t> bytes)
    {
        int number = 0;
        int size = bytes.size();
        for (int i = 0; i < bytes.size(); i++)
        {
            number |= bytes[size - i - 1] << (i * 8);
        }
        return number;
    }

    std::string padByte(int num, int pad)
    {
        std::string str = std::to_string(num);
        int len = str.length();
        if (len < pad)
        {
            str.insert(0, pad - len, ' ');
        }
        return str;
    }

    // std::string colorByRGB(int r, int g, int b, bool fg)
    // {
    //     std::stringstream ss;
    //     ss << "\x1B[" << (fg ? 3 : 4) << "8;2;" << r << ";" << g << ";" << b << "m";
    //     return ss.str();
    // }

    // std::vector<float> linearInterpolation(float start, float end, int n)
    // {
    //     std::vector<float> values(n);
    //
    //     float step = (end - start) / (n - 1);
    //     for (int i = 0; i < n; i++)
    //     {
    //         values[i] = start + i * step;
    //     }
    //
    //     return values;
    // }

    std::vector<float> linearInterpolation(float start, float end, uint32_t n, bool mirror)
    {
        if (n <= 0)
            return {};

        // If mirrored, we only generate half and reflect
        const uint32_t baseCount = mirror ? (n + 1) / 2 : n;

        std::vector<float> base(baseCount);

        const float step = (end - start) / (static_cast<float>(baseCount) - 1.f);
        for (int i = 0; i < baseCount; i++)
        {
            base[i] = start + i * step;
        }

        if (!mirror)
            return base;

        std::vector<float> result;
        result.reserve(n);

        // Forward
        for (float v : base)
            result.push_back(v);

        // Backward (skip first and last to avoid duplicates)
        for (int i = base.size() - 2; i > 0; i--)
            result.push_back(base[i]);

        return result;
    }

    std::vector<uint16_t> make_range(uint16_t start, uint16_t end)
    {
        auto view = std::ranges::iota_view(start, end); // [start, end)
        return {view.begin(), view.end()};
    };

    // Generate gradient
    // std::vector<std::vector<int>> getGradient(int numElements, std::vector<std::vector<int>> colors, std::vector<float> percentages)
    // {
    //     std::vector<HSV> hsvColors;
    //     for (auto& c : colors)
    //     {
    //         hsvColors.push_back(rgbToHsv(c));
    //     }

    //     std::vector<std::vector<int>> gradient;
    //     gradient.resize(numElements);

    //     for (int i = 0; i < numElements; ++i)
    //     {
    //         float t = (float)i / (numElements - 1);
    //         int segment = 0;

    //         while (segment < percentages.size() - 1 && t > percentages[segment + 1])
    //         {
    //             segment++;
    //         }

    //         float localT = (t - percentages[segment]) / (percentages[segment + 1] - percentages[segment]);
    //         gradient[i] = hsvToRgb(lerpHsv(hsvColors[segment], hsvColors[segment + 1], localT));
    //     }

    //     return gradient;
    // }

    // std::vector<std::vector<int>> getGradient(int numElements, std::vector<std::vector<int>> colors, std::vector<float> percentages)
    // {
    //     if (percentages.size() == 0)
    //     {
    //         for (int i = 0; i < colors.size(); i++)
    //         {
    //             percentages.push_back(1.f / colors.size());
    //         }
    //     }
    //     if (percentages.size() != colors.size() || percentages.size() > numElements)
    //         return {};

    //     std::vector<std::vector<int>> gradient(numElements, std::vector<int>(3, 0));
    //     // auto percentagesMod = percentages;
    //     // percentagesMod[0]
    //     int start = 0;
    //     colors.push_back(colors[0]);

    //     for (int i = 0; i < colors.size() - 2; i++)
    //     {
    //         auto nextCol = colors[i + 1];
    //         auto hsv1 = rgbToHsv(colors[i]);
    //         auto hsv2 = rgbToHsv(colors[i]);

    //         // int diff = colors.size() /
    //     }

    //     // percentages[0] /= 2.f;
    //     // percentages.push_back(percentages[0]);

    //     // for (int i = 0; i < colors.size() - 1; i++)
    //     // {
    //     //     float percentage = percentages[i];
    //     //     int end = start + percentage * numElements;
    //     //     if (i == colors.size() - 2)
    //     //     end = numElements;
    //     //     auto &startColor = colors[i];
    //     //     std::vector<int> &endColor = i == colors.size() - 1 ? colors[i] : colors[i + 1];
    //     //     #include <stdio.h>
    //     //     for (int j = start; j < end; j++)
    //     //     {
    //     //         float ratio = (j - start) / (float)(end - start - 1);
    //     //         for (int k = 0; k < 3; k++)
    //     //         {
    //     //             gradient[j][k] = startColor[k] * (1 - ratio) + endColor[k] * ratio;
    //     //             printf("%d, ", gradient[j][k]);
    //     //         }
    //     //         printf("\n");
    //     //         // std::cout << "Here" << std::endl;
    //     //     }
    //     //     start += percentage * numElements;
    //     // }

    //     return gradient;
    // }

    /*

    for (int i = 0; i < colors.size() - 1; i++)
        {
            float percentage = percentages[i];
            int end = start + percentage * numElements;
            if (i == colors.size() - 2)
                end = numElements;
            auto &startColor = colors[i];
            std::vector<int> &endColor = i == colors.size() - 1 ? colors[i] : colors[i + 1];
            for (int j = start; j < end; j++)
            {
                float ratio = (j - start) / (float)(end - start - 1);
                for (int k = 0; k < 3; k++)
                {
                    gradient[j][k] = startColor[k] * (1 - ratio) + endColor[k] * ratio;
                }
            }
            start += percentage * numElements;
        }*/

    // // hsv gradient implementation
    // std::vector<std::vector<int>> getGradientHsv(int numElements, std::vector<std::vector<int>> colors, std::vector<float> percentages)
    // {
    //     if (percentages.size() != colors.size() || percentages.size() > numElements)
    //         return {};

    //     std::vector<std::vector<int>> gradient(numElements, std::vector<int>(3, 0));
    //     int start = 0;
    //     for (int i = 0; i < colors.size() - 1; i++)
    //     {
    //         float percentage = percentages[i];
    //         int end = start + percentage * numElements;
    //         if (i == colors.size() - 2)
    //             end = numElements;
    //         auto &startColor = colors[i];
    //         std::vector<int> &endColor = i == colors.size() - 1 ? colors[i] : colors[i + 1];

    //         auto startHsv = rgbToHsv(startColor);
    //         auto endHsv = rgbToHsv(endColor);

    //         for (int j = start; j < end; j++)
    //         {
    //             float ratio = (j - start) / (float)(end - start - 1);
    //             gradient[j] = hsvToRgb({startHsv[0] * (1 - ratio) + endHsv[0] * ratio,
    //                                     startHsv[1] * (1 - ratio) + endHsv[1] * ratio,
    //                                     startHsv[2] * (1 - ratio) + endHsv[2] * ratio});
    //             // for (int k = 0; k < 3; k++)
    //             // {
    //             //     gradient[j][k] = startColor[k] * (1 - ratio) + endColor[k] * ratio;
    //             // }
    //         }
    //         start += percentage * numElements;
    //     }

    //     return gradient;
    // }

};
