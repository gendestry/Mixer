#include "Colors.h"
#include <algorithm>
#include <cmath>
#include <format>
#include <sstream>

namespace Utils
{
    const std::string Colors::colorGreen = "\x1B[32m";
    const std::string Colors::colorYellow = "\x1B[33m";
    const std::string Colors::colorBlue = "\x1B[34m";
    const std::string Colors::colorReset = "\x1B[0m";
    const std::string Colors::colorDim = "\x1B[2m";
    const std::string Colors::colorItalic = "\x1B[3m";

    std::string Colors::RGB::toString() const
    {
        return std::format("R: {}, G: {}, B {}\n", r, g, b);
    }

    std::string Colors::HSV::toString() const
    {
        return std::format("Hue: {}, Sat: {}, Val {}\n", h, s, v);
    }

    Colors::HSV Colors::rgbToHsv(const Colors::RGB& rgb)
    {
        // Normalize RGB to [0, 1]
        float r = rgb[0] / 255.0f;
        float g = rgb[1] / 255.0f;
        float b = rgb[2] / 255.0f;

        // Find min and max values of R, G, B
        float maxVal = std::max({r, g, b});
        float minVal = std::min({r, g, b});
        float delta = maxVal - minVal;

        // Compute Hue
        float h = 0.0f;
        if (delta > 0.00001f)
        { // Avoid division by zero
            if (maxVal == r)
            {
                h = 60.0f * (fmod(((g - b) / delta), 6.0f));
            }
            else if (maxVal == g)
            {
                h = 60.0f * (((b - r) / delta) + 2.0f);
            }
            else if (maxVal == b)
            {
                h = 60.0f * (((r - g) / delta) + 4.0f);
            }
        }
        if (h < 0.0f)
            h += 360.0f; // Ensure hue is positive

        // Compute Saturation
        float s = (maxVal > 0.00001f) ? (delta / maxVal) : 0.0f;

        // Compute Value
        float v = maxVal;

        return {h, s, v};
    }

    Colors::RGB Colors::hsvToRgb(const Colors::HSV& hsv)
    {
        float h = hsv[0]; // Hue in degrees
        float s = hsv[1]; // Saturation in [0, 1]
        float v = hsv[2]; // Value in [0, 1]

        h = std::fmod(h, 720.f);
        h = h > 360.f ? 720.f - h : h;

        float r = 0.0f, g = 0.0f, b = 0.0f;

        int hi = static_cast<int>(std::floor(h / 60.0f)) % 6; // Sector of the color wheel (0-5)
        float f = (h / 60.0f) - hi;                           // Fractional part of h/60
        float p = v * (1.0f - s);
        float q = v * (1.0f - f * s);
        float t = v * (1.0f - (1.0f - f) * s);

        switch (hi)
        {
        case 0:
            r = v, g = t, b = p;
            break;
        case 1:
            r = q, g = v, b = p;
            break;
        case 2:
            r = p, g = v, b = t;
            break;
        case 3:
            r = p, g = q, b = v;
            break;
        case 4:
            r = t, g = p, b = v;
            break;
        case 5:
            r = v, g = p, b = q;
            break;
        }

        // Convert to 0-255 range
        return {static_cast<uint8_t>(r * 255.0f),
                static_cast<uint8_t>(g * 255.0f),
                static_cast<uint8_t>(b * 255.0f)};
    }

    std::string Colors::colorByRGB(uint8_t r, uint8_t g, uint8_t b, bool fg)
    {
        std::stringstream ss;
        ss << "\x1B[" << (fg ? 3 : 4) << "8;2;" << r << ";" << g << ";" << b << "m";
        return ss.str();
    }
    
    std::string Colors::colorByRGB(const Colors::RGB& color, bool fg)
    {
        return std::format("\x1B[{}8;2;{};{};{}m", (fg ? 3 : 4), color.r, color.g, color.b);
    }

    std::vector<Colors::RGB> Colors::makeGradient(const std::vector<Colors::RGB>& colors, const std::vector<float>& weights, int N)
    {
        if (colors.size() != weights.size())
            throw std::invalid_argument("Colors and weights must match.");

        if (colors.size() < 2)
            throw std::invalid_argument("Need at least two colors.");

        if (N < colors.size())
            throw std::invalid_argument("N must be > 1.");

        // Normalize weights
        float sum = 0.0f;
        for (float w : weights)
            sum += w;

        if (sum <= 0)
            throw std::invalid_argument("Invalid weights.");

        std::vector<float> norm(weights.size());
        for (size_t i = 0; i < weights.size(); i++)
            norm[i] = weights[i] / sum;

        // Build stops
        std::vector<float> stops;
        stops.reserve(colors.size());

        float pos = 0.0f;

        for (float w : norm) {
            stops.push_back(pos);
            pos += w;
        }

        stops.push_back(1.0f); // End

        std::vector<Colors::RGB> result;
        result.reserve(N);

        for (int i = 0; i < N; i++) {

            float t = float(i) / float(N - 1);

            // Find stop
            size_t idx = 0;
            while (idx + 1 < stops.size() && t > stops[idx + 1])
                idx++;

            // Clamp
            if (idx >= colors.size() - 1) {
                result.push_back(colors.back());
                continue;
            }

            float start = stops[idx];
            float end   = stops[idx + 1];

            float localT = (t - start) / (end - start);

            // Interpolate only in this region
            const auto& c1 = colors[idx];
            const auto& c2 = colors[idx + 1];

            uint8_t r = uint8_t(c1.r + (c2.r - c1.r) * localT);
            uint8_t g = uint8_t(c1.g + (c2.g - c1.g) * localT);
            uint8_t b = uint8_t(c1.b + (c2.b - c1.b) * localT);

            result.emplace_back(r, g, b);
        }

        return result;
    }


};