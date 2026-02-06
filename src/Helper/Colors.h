#pragma once
#include <cstdint>
#include <string>
#include <vector>


namespace Utils
{
    class Colors
    {
    public:
        struct RGB
        {
            union
            {
                struct 
                {
                    uint8_t r,g,b;
                };

                uint8_t rgb[3];
            };

            float pr, pg, pb;

            RGB() = default;

            RGB(uint8_t red, uint8_t green, uint8_t blue)
            {
                r = red;
                g = green;
                b = blue;

                toPercentage();
            }

            RGB& toPercentage() {
                pr = (static_cast<float>(r) / 255.0f) * 100.0f;
                pg = (static_cast<float>(g) / 255.0f) * 100.0f;
                pb = (static_cast<float>(b) / 255.0f) * 100.0f;

                return *this;
            }

            uint8_t& operator[](uint8_t index)
            {
                return rgb[index];
            }

            uint8_t operator[](uint8_t index) const
            {
                return rgb[index];
            }

            std::string toString() const;
        };

        struct HSV
        {
            union
            {
                struct 
                {
                    float h,s,v;
                };
                float hsv[3];
            };

            HSV() = default;

            HSV(float hue, float saturation, float value)
            {
                h = hue;
                s = saturation;
                v = value;
            }

            float& operator[](uint8_t index)
            {
                return hsv[index];
            }

            float operator[](uint8_t index) const
            {
                return hsv[index];
            }

            std::string toString() const;
        };

        static HSV rgbToHsv(const RGB& rgb);
        static RGB hsvToRgb(const HSV& hsv);

        static std::string colorByRGB(uint8_t r, uint8_t g, uint8_t b, bool fg);
        static std::string colorByRGB(const RGB& color, bool fg);
        static std::vector<RGB> gradient(const RGB& color1, const RGB& color2);
        static std::vector<RGB> makeGradient(const std::vector<RGB>& colors, const std::vector<float>& weights, int N);

        
        static const std::string colorGreen;
        static const std::string colorYellow;
        static const std::string colorBlue;
        static const std::string colorReset;
        static const std::string colorDim;
        static const std::string colorItalic;
    };
    // const std::string colorGreen = "\x1B[32m";
    // const std::string colorYellow = "\x1B[33m";
    // const std::string colorBlue = "\x1B[34m";
    // const std::string colorReset = "\x1B[0m";
    // const std::string colorDim = "\x1B[2m";
    // const std::string colorItalic = "\x1B[3m";

    

  
};